#include <algorithm>
#include <cmath>
#include "Matrices.h"

#include "ZBuffer.h"

ZBuffer::ZBuffer() :
	_bits(nullptr),
	_z(),
	_drawn(),
	_width(0),
	_height(0),
	_lights(MAX_LIGHTS),
	_base_color(),
	_attr({}),
	_ambient(),
	_Ia(1.0),
	_Id(1.0),
	_Is(1.0),
	_Ispot(1.0),
	_curr_polygon_normal({}),
	_curr_polygon_pos({}),
	_png(),
	_is_foggy(false),
	_fog_color({ 127, 127, 127 }),
	_has_background_image(false),
	_spot_light_theta(7.0),
	_min_fog(1.0),
	_max_fog(1.0),
	_phong_factor(1)
{
	setDefaultColor(RGB(0, 0, 0));
}

ZBuffer::ZBuffer(const uint &width, const uint &height, const Color &color) :
	_bits(new int[width * height]),
	_width(width),
	_height(height),
	_lights(MAX_LIGHTS),
	_base_color(),
	_attr({}),
	_ambient(),
	_Ia(1.0),
	_Id(1.0),
	_Is(1.0),
	_Ispot(1.0),
	_curr_polygon_normal({}),
	_curr_polygon_pos({}),
	_png(),
	_is_foggy(false),
	_fog_color({ 127, 127, 127 }),
	_has_background_image(false),
	_spot_light_theta(7.0),
	_min_fog(1.0),
	_max_fog(1.0),
	_phong_factor(1)
{
	_z.resize(width * height);
	_drawn.resize(width * height);
	setDefaultColor(color);
	_Ia = 0.2;
}

void ZBuffer::saveImageAsPng(const char* name)
{
	_png.SetWidth(_width);
	_png.SetHeight(_height);
	_png.SetFileName(name);

	auto res = _png.InitWritePng();

	for (int i = 0; i < _width * _height; i++)
	{
		int x = i % _width;
		int y = _height - 1 - i / _width;

		int r = GetRValue(_bits[i]);
		int g = GetGValue(_bits[i]);
		int b = GetBValue(_bits[i]);

		Color color = SET_RGB(b,g,r);

		_png.SetValue(x, y, color);
	}

	res = _png.WritePng();
}

void ZBuffer::saveImageAsPng(const char* name, const int *bits)
{
	_png.SetWidth(_width);
	_png.SetHeight(_height);
	_png.SetFileName(name);

	auto res = _png.InitWritePng();

	for (int i = 0; i < _width * _height; i++)
	{
		int x = i % _width;
		int y = _height - 1 - i / _width;

		int r = GetRValue(bits[i]);
		int g = GetGValue(bits[i]);
		int b = GetBValue(bits[i]);

		Color color = SET_RGB(b, g, r);

		_png.SetValue(x, y, color);
	}

	res = _png.WritePng();
}

ZBuffer &ZBuffer::resize(const uint &width, const uint &height)
{
	if (!isEmpty())
	{
		delete[] _bits;
		_bits = new int[width * height];
		_z.resize(width * height);
		_drawn.resize(width * height);
		_width = width;
		_height = height;
	}

	return *this;
}

ZBuffer &ZBuffer::set(const Pixel &p)
{
	uint pos = p.x + _width * p.y;

	if (pos >= _width * _height)
	{
		return *this;
	}

	if (!_drawn[pos])
	{
		_bits[pos] = RGBToBGR(p.color);
		_z[pos] = p.depth;
		_drawn[pos] = true;
		return *this;
	}
	else
	{
		if (p.depth > _z[pos] && _bits[pos] != RGBToBGR(p.color))
		{
			_bits[pos] = RGBToBGR(p.color);
			_z[pos] = p.depth;
		}
	}

	return *this;
}

Color ZBuffer::RGBToBGR(COLORREF col)
{
	return RGB(GetBValue(col), GetGValue(col), GetRValue(col));
}

Color ZBuffer::BGRToRGB(COLORREF col)
{
	return RGB(GetBValue(col), GetGValue(col), GetRValue(col));
}

ZBuffer &ZBuffer::setDefaultColor(const Color &color)
{
	for (uint i = 0; i < _width * _height; i++)
	{
		_bits[i] = RGBToBGR(color);
	}

	_color = color;

	return *this;
}

int ZBuffer::calcQuarter(const Pixel &p2, const Pixel &p, const double &m)
{
	if (std::abs(m) < 1.0)
	{
		if (m > 0)
		{
			if (p.x <= p2.x)
			{
				return 1;
			}
			else
			{
				return 7;
			}
		}
		else
		{
			if (p.x <= p2.x)
			{
				return 3;
			}
			else
			{
				return 5;
			}
		}
	}
	else
	{
		if (m > 0)
		{
			if (p.x <= p2.x)
			{
				return 2;
			}
			else
			{
				return 8;
			}
		}
		else
		{
			if (p.x <= p2.x)
			{
				return 4;
			}
			else
			{
				return 6;
			}
		}
	}
}

double ZBuffer::calcWeight(const Pixel &p1, const Pixel &p2, const Pixel &p)
{
	double line_dist = std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
	double dist_from_begin = std::sqrt(std::pow(p.x - p1.x, 2) + std::pow(p.y - p1.y, 2));
	double res = 0.0;

	if (line_dist == 0.0)
	{
		//res = std::abs(p.depth - p2.depth) < 1e-3 ? 1.0 : 0.0;
		res = 0.0;
	}
	else
	{
		res = dist_from_begin / line_dist;
	}

	return res;
}

double ZBuffer::calcDepthInFog(const double &depth)
{
	if (!_is_foggy)
	{
		return 0.0;
	}

	double fog_start = _min_fog;
	double fog_end = _max_fog;
	double res = (depth - fog_start) / (fog_end - fog_start);

	if (res < 0.0)
	{
		res = 0.0;
	}
	if (res > 1.0)
	{
		res = 1.0;
	}

	return res;
}

Pixel ZBuffer::interpolatePixel(const Pixel &p1, const Pixel &p2, const Pixel &p)
{
	double weight = calcWeight(p1, p2, p);

	auto depth = weight * p2.depth + (1 - weight) * p1.depth;
	auto normal = p2.normal * weight + p1.normal * (1 - weight);
	auto pos = p2.pos * weight + p1.pos * (1 - weight);

	if (_attr.shading == Shading::FLAT)
	{
		pos = _curr_polygon_pos;
	}

	Vec3 ambient = calcAmbient();
	Vec3 diffuse = calcDiffuse(pos, normal);
	Vec3 specular = calcSpecular(pos, normal);
	Vec3 spot_light = calcSpotLight(pos);

	Vec3 object_color = ambient + diffuse + specular + spot_light;

	auto depth_in_fog = calcDepthInFog(depth);

	Vec3 final_color = _fog_color * depth_in_fog + object_color * (1 - depth_in_fog);

	Color color = vecToColor(final_color);
	
	return { p.x, p.y, depth, color, normal, pos };
}

Vec3 ZBuffer::calcSpotLight(const Vec3 &pos)
{
	for (auto &light : _lights)
	{
		if (light.type == LIGHT_TYPE_SPOT)
		{
			auto light_color = getLightColor(light);

			Vec3 light_dir = { light.dirX, light.dirY, light.dirZ };
			light_dir.normalize();

			Vec3 light_pos = { light.posX, light.posY, light.posZ };
			auto angle = _spot_light_theta * consts::PI / 180.0;

			auto res = acos(light_dir.dot((pos - light_pos).normalize()));

			if (res >= 0 && res <= angle)
			{
				auto base_color = colorToVec(_base_color);
				auto k_spot = base_color.elementMultiply(light_color) / 255.0;
				return k_spot * _Ispot;
			}
		}
	}

	return {};
}

Vec3 ZBuffer::calcLightDir(const LightParams &light, const Vec3 &pos)
{
	if (light.type == LightType::LIGHT_TYPE_DIRECTIONAL)
	{
		return { light.dirX, light.dirY, light.dirZ };
	}
	else if (light.type == LightType::LIGHT_TYPE_POINT)
	{
		Vec3 light_pos = { light.posX,  light.posY, light.posZ };
		Vec3 light_dir = light_pos - pos;
		light_dir.normalize();

		return light_dir;
	}
}

Vec3 ZBuffer::calcAmbient()
{
	Vec3 base_color = colorToVec(_base_color);
	Vec3 ambient = getLightColor(_ambient);
	Vec3 Ka = base_color.elementMultiply(ambient) / 255.0;

	return Ka * _Ia;
}

Vec3 ZBuffer::calcDiffuse(const Vec3 &pos, const Vec3 &normal)
{
	if (!_lights.empty())
	{
		Vec3 res = {};

		for (auto &light : _lights)
		{
			if (light.type != LIGHT_TYPE_SPOT)
			{
				Vec3 light_dir = calcLightDir(light, pos);
				double cos_theta = max(normal.dot(light_dir), 0.0);

				Vec3 base_color = colorToVec(_base_color);
				Vec3 diffuse = getLightColor(light);

				Vec3 Kd = base_color.elementMultiply(diffuse) / 255.0;

				res = res + Kd * _Id * cos_theta;
			}
		}

		return res;
	}

	return {};
}

Vec3 ZBuffer::calcSpecular(const Vec3 &pos, const Vec3 &normal)
{
	if (!_lights.empty())
	{
		Vec3 res = {};

		for (auto &light : _lights)
		{
			if (light.type != LIGHT_TYPE_SPOT)
			{
				Vec3 view_pos = { _attr.view_pos(0), _attr.view_pos(1), _attr.view_pos(2) };
				Vec3 view_dir = view_pos - pos;
				view_dir.normalize();

				Vec3 light_dir = calcLightDir(light, pos);

				Vec3 reflect = -light_dir - normal * 2 * (-light_dir.dot(normal));
				reflect.normalize();

				double cos_theta = max(view_dir.dot(reflect), 0.0);

				Vec3 base_color = colorToVec(_base_color);
				Vec3 specular = getLightColor(light);

				Vec3 Ks = base_color.elementMultiply(specular) / 255.0;

				res = res + Ks * _Is * std::pow(cos_theta, _phong_factor);
			}
		}

		return res;
	}

	return {};
}

void ZBuffer::setLights(const std::vector<LightParams> &lights)
{
	_lights = lights;
	allocateLightsIntensities();
}

void ZBuffer::allocateLightsIntensities()
{
	if (_lights.empty())
	{
		_Ia = 1.0;
	}

	if (!_lights.empty())
	{
		int spot_lights_count = 0;

		for (auto &light : _lights)
		{
			if (light.type == LIGHT_TYPE_SPOT)
			{
				spot_lights_count++;
			}
		}

		int non_spot_light_count = _lights.size() - spot_lights_count;

		if (spot_lights_count == _lights.size())
		{
			_Is = 0;
			_Id = 0;
			_Ispot = (1.0 - _Ia) / spot_lights_count;
		}
		else if (non_spot_light_count == _lights.size())
		{
			_Is = (1.0 - _Ia) / _lights.size() / 2.0;
			_Id = (1.0 - _Ia) / _lights.size() / 2.0;
			_Ispot = 0;
		}
		else
		{
			_Is = (1.0 - _Ia) / _lights.size() / 2.0;
			_Id = (1.0 - _Ia) / _lights.size() / 2.0;
			_Ispot = 1 - _Ia - (1 - _Ia) / _lights.size();
		}
	}
}

void ZBuffer::setBackgroundImage(const char *file_name, const BackgroundImageType &type)
{
	_png.SetFileName(file_name);
	auto res = _png.ReadPng();

	auto w = _png.GetWidth();
	auto h = _png.GetHeight();
	auto ch = _png.GetNumChannels();

	if (type == BackgroundImageType::REPEAT)
	{
		repeatImage(w, h);
	}
	else if (type == BackgroundImageType::STRETCH)
	{
		stretchImage(w, h);
	}
}

void ZBuffer::repeatImage(const int &w, const int &h)
{
	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			auto color = getValue(i, j);
			if (color.x == 0 && color.y == 0 && color.z == 0)
			{
				auto val = _png.GetValue(j % w, i % h);
				auto r = GET_R(val);
				auto g = GET_G(val);
				auto b = GET_B(val);

				setValue(i, j, RGB(r, g, b));
			}

		}
	}
}

void ZBuffer::stretchImage(const int &w, const int &h)
{
	double w_ratio = static_cast<double>(_width) / w;
	double h_ratio = static_cast<double>(_height) / h;

	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			auto color = getValue(i, j);
			if (color.x == 0 && color.y == 0 && color.z == 0)
			{
				auto val = _png.GetValue(j / w_ratio, i / h_ratio);
				auto r = GET_R(val);
				auto g = GET_G(val);
				auto b = GET_B(val);

				setValue(i, j, RGB(r, g, b));
			}


		}
	}
}

Color ZBuffer::vecToColor(const Vec3 &vec)
{
	return RGB(vec.x, vec.y, vec.z);
}

Vec3 ZBuffer::colorToVec(const Color &color)
{
	return { static_cast<double>(GetRValue(color)),
		static_cast<double>(GetGValue(color)),
		static_cast<double>(GetBValue(color)) };
}

Vec3 ZBuffer::getLightColor(const LightParams &light) const
{
	return { static_cast<double>(light.colorR),
	static_cast<double>(light.colorG),
	static_cast<double>(light.colorB) };
}

Pixel ZBuffer::nextPixel(const Pixel &p1, const Pixel &p2, const Pixel &p)
{
	int dy = p2.y - p1.y;
	int dx = p2.x - p1.x;
	int x = -1;
	int y = -1;
	Pixel res = {};

	if (p2.y == p.y && p2.x == p.x)
	{
		x = p.x;
		y = p.y;
	}
	else if (p2.y == p.y && p2.x > p.x)
	{
		x = p.x + 1;
		y = p.y;
	}
	else if (p2.y == p.y && p2.x < p.x)
	{
		x = p.x - 1;
		y = p.y;
	}
	else if (p2.x == p.x && p2.y > p.y)
	{
		x = p.x;
		y = p.y + 1;
	}
	else if (p2.x == p.x && p2.y < p.y)
	{
		x = p.x;
		y = p.y - 1;
	}
	else
	{
		double m = static_cast<double>(dy) / static_cast<double>(dx);

		int quarter = calcQuarter(p2, p, m);

		if (quarter == 1 || quarter == 3 || quarter == 5 || quarter == 7)
		{
			x = quarter < 4 ? p.x + 1 : p.x - 1;
			y = m * (x - p1.x) + p1.y;
		}
		else
		{
			y = quarter == 2 || quarter == 6 ? p.y + 1 : p.y - 1;
			x = (1 / m) * (y + m * p1.x - p1.y);
		}
	}

	res.x = x;
	res.y = y;
	res = interpolatePixel(p1, p2, res);

	return res;
}

void ZBuffer::drawPolygonWireFrame(const Poly &polygon)
{
	_base_color = polygon.getColor();
	auto &vertices = polygon.getVertices();
	Pixel first_pixel = {};
	Pixel last_pixel = {};

	for (int i = 0; i < vertices.size() - 1; i++)
	{
		auto px1 = toPixel(*vertices[i], polygon);
		auto px2 = toPixel(*vertices[i + 1], polygon);
		drawLine(px1, px2);

		if (i == 0)
		{
			first_pixel = px1;
		}
		if (i == vertices.size() - 2)
		{
			last_pixel = px2;
		}
	}
	drawLine(first_pixel, last_pixel);
}

void ZBuffer::calcDrawOrder(Pixel &start, Pixel &via, Pixel &target, 
	const Poly &polygon)
{
	auto vertices = polygon.getVertices();
	std::vector<Pixel> pixels;
	for (auto &vertex : vertices)
	{
		pixels.push_back(toPixel(*vertex, polygon));
	}

	std::sort(pixels.begin(), pixels.end(), [](const Pixel &p1, const Pixel &p2)
	{
		if (p1.y == p2.y)
		{
			return p1.depth > p2.depth;
		}

		return p1.y < p2.y;
	});

	start = pixels[0];
	via = pixels[1];
	target = pixels[2];
}

Pixel ZBuffer::nextPixelFill(const Pixel &start, const Pixel &target, const Pixel &p)
{
	auto curr = p;
	int y = curr.y;

	do
	{
		y = curr.y;
		curr = nextPixel(start, target, curr);
	} while (curr.y - y != 1 && curr.y != target.y);

	if (curr.y == target.y)
	{
		curr.x = target.x;
	}

	return curr;
}

void ZBuffer::calcCurrPolygonNormal(const Poly &polygon)
{
	auto raw_polygon_normal = polygon.getCalcFaceNormal().toHomogeneous();
	raw_polygon_normal(3) = 0.0;

	auto polygon_normal = _attr.T * raw_polygon_normal;

	_curr_polygon_normal = { polygon_normal(0), polygon_normal(1), polygon_normal(2) };
	_curr_polygon_normal.normalize();
}

void ZBuffer::calcCurrPolygonPos(const Poly &polygon)
{
	auto polygon_pos = _attr.T * polygon.getPos().toHomogeneous();
	polygon_pos /= polygon_pos(3);
	_curr_polygon_pos = { polygon_pos(0), polygon_pos(1), polygon_pos(2) };
}

void ZBuffer::drawPolygonSolid(const Poly &polygon)
{
	_base_color = polygon.getColor();

	Pixel start = {};
	Pixel via = {};
	Pixel target = {};
	bool to_via = true;
	bool is_filled = false;

	calcDrawOrder(start, via, target, polygon);

	auto curr1 = start.y == via.y ? via: start;
	auto curr2 = start;
	to_via = curr1.y != via.y;
	
	curr1 = interpolatePixel(start, via, curr1);
	curr2 = interpolatePixel(start, target, curr2);

	drawLine(curr1, curr2);

	while (!is_filled)
	{
		curr1 = to_via == true ? nextPixelFill(start, via, curr1) : nextPixelFill(via, target, curr1);
		curr2 = nextPixelFill(start, target, curr2);

		if (to_via && curr1.y == via.y)
		{
			to_via = false;
		}

		drawLine(curr1, curr2);

		if (curr1 == target && curr2 == target)
		{
			is_filled = true;
		}
	}
}

void ZBuffer::draw(const Object &object)
{
	for (auto &mesh : object.getMeshes())
	{
		if (_attr.drawing_mode == DrawingMode::WIRE_FRAME)
		{
			for (auto &polygon : mesh.getRawPolygons())
			{
				drawPolygonWireFrame(*polygon);
			}
		}
		else
		{
			for (auto &polygon : mesh.getPolygons())
			{
				calcCurrPolygonNormal(*polygon);
				calcCurrPolygonPos(*polygon);
				if (_attr.back_face_culling)
				{
					if (_curr_polygon_normal.dot(Vec3{ 0.0, 0.0, -1.0 }) < 0)
					{
						drawPolygonSolid(*polygon);
					}
				}
				else
				{
					drawPolygonSolid(*polygon);
				}
			}
		}
	}
}

void ZBuffer::drawLine(const Pixel &p1, const Pixel &p2)
{
	Pixel curr = p1;
	set(curr);

	while (curr != p2)
	{
		curr = nextPixel(p1, p2, curr);
		set(curr);
	}
}

Vec3 ZBuffer::calcVertexNormal(const Vertex &vertex)
{
	if (_attr.shading == Shading::FLAT)
	{
		return _curr_polygon_normal;
	}

	else
	{
		auto raw_normal = vertex.calc_normal.toHomogeneous();
		raw_normal(3) = 0.0;

		auto T_normal = _attr.T * raw_normal;

		Vec3 normal = { T_normal(0), T_normal(1), T_normal(2) };

		return normal.normalize();
	}
}

Vec3 ZBuffer::calcVertexPos(const Vertex &vertex)
{
	auto raw_pos = _attr.T * vertex.pos.toHomogeneous();
	raw_pos /= raw_pos(3);

	return { raw_pos(0), raw_pos(1), raw_pos(2) };
}

Pixel ZBuffer::toPixel(const Vertex &vertex, const Poly &polygon)
{
	auto pos = calcVertexPos(vertex);
	auto normal = calcVertexNormal(vertex);
	
	int x_res = static_cast<uint>((_width / 2.0) * (pos.x + 1.0));
	int y_res = static_cast<uint>(-(_height / 2.0) * (pos.y - 1.0));

	return { x_res, y_res, pos.z, vertex.color, normal, pos};
}

Vec3 ZBuffer::getValue(const int &row, const int &col)
{
	if (row < 0 || row >= _height || col < 0 || col >= _width)
	{
		return { 0, 0, 0 };
	}

	return colorToVec(_bits[row * _width + col]);
}

void ZBuffer::setValue(const int &row, const int &col, const Color &color)
{
	_bits[row * _width + col] = RGBToBGR(color);
}

Vec3 ZBuffer::calcFilterColor(const int &row, const int &col, const std::vector<int> &kernel)
{
	if (vecToColor(getValue(row, col)) != 0)
	{
		return getValue(row, col);
	}

	Vec3 res = { 0, 0, 0 };
	int kernel_size = sqrt(kernel.size());

	for (int i = 0; i < kernel_size; i++)
	{
		for (int j = 0; j < kernel_size; j++)
		{
			res = res + getValue(row - kernel_size / 2 + i, col - kernel_size / 2 + j) * kernel[i * kernel_size + j];
		}
	}

	int sum = 0;
	for (auto &val : kernel)
	{
		sum += val;
	}

	return res / sum;
}

void ZBuffer::applyFilter(const std::vector<int> &kernel)
{
	int* new_bits = new int[_width * _height];

	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			auto filter_color = calcFilterColor(i, j, kernel);

			Color final_color = RGBToBGR(vecToColor(filter_color));

			//if (filter_color.x == 255 && filter_color.y == 255 && filter_color.z == 255)
			//{
			//	final_color = 0;
			//}

			new_bits[i * _width + j] = final_color;
		}
	}

	_bits = new_bits;
}

ZBuffer::~ZBuffer()
{
	if (!isEmpty())
	{
		delete[] _bits;
	}
}
