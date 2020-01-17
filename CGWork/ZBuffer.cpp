#include "ZBuffer.h"

ZBuffer::ZBuffer() :
	_bits(nullptr),
	_z(),
	_drawn(),
	_width(0),
	_height(0),
	_lights(MAX_LIGHTS)
{
	setDefaultColor(RGB(0, 0, 0));
}

ZBuffer::ZBuffer(const uint &width, const uint &height, const Color &color) :
	_bits(new int[width * height]),
	_width(width),
	_height(height)
{
	_z.resize(width * height);
	_drawn.resize(width * height);
	setDefaultColor(color);
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

	//Vec3d light_pos(0.0, 0.0, 1.0);
	//Vec3d normal(p.normal(0), p.normal(1), p.normal(2));
	//Vec3d light_dir(light_pos(0) - p.pos(0), light_pos(1) - p.pos(1), light_pos(2) - p.pos(2));
	//light_dir.normalize();

	//auto test = normal.dot(light_dir);

	//auto r = GetRValue(p.color) * test;
	//auto g = GetGValue(p.color) * test;
	//auto b = GetBValue(p.color) * test;

	//Color color(RGB(r, g, b));

	if (!_drawn[pos])
	{
		_bits[pos] = RGBToBGR(p.color);
		_z[pos] = p.depth;
		_drawn[pos] = true;
		return *this;
	}
	else
	{
		if (p.depth > _z[pos])
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

Pixel ZBuffer::interpolatePixel(const Pixel &p1, const Pixel &p2, const Pixel &p)
{
	double line_dist = std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
	double dist_from_begin = std::sqrt(std::pow(p.x - p1.x, 2) + std::pow(p.y - p1.y, 2));

	if (p.x == p1.x && p.y == p1.y)
	{
		return p1;
	}

	if (p.x == p2.x && p.y == p2.y)
	{
		return p2;
	}

	double a = dist_from_begin / line_dist;

	auto depth = a * p2.depth + (1 - a) * p1.depth;

	return { p.x, p.y, depth, p.color };
}

Pixel ZBuffer::nextPixel(const Pixel &p1, const Pixel &p2, const Pixel &p)
{
	int dy = p2.y - p1.y;
	int dx = p2.x - p1.x;
	int x = 1;
	int y = -1;
	Pixel res = interpolatePixel(p1, p2, p);

	if (p2.y == p.y && p2.x > p.x)
	{
		res.x = p.x + 1;
		res.y = p.y;
		return res;
	}
	if (p2.y == p.y && p2.x < p.x)
	{
		res.x = p.x - 1;
		res.y = p.y;
		return res;
	}
	if (p2.x == p.x && p2.y > p.y)
	{
		res.x = p.x;
		res.y = p.y + 1;
		return res;
	}
	if (p2.x == p.x && p2.y < p.y)
	{
		res.x = p.x;
		res.y = p.y - 1;
		return res;
	}

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

	res.x = x;
	res.y = y;
	return res;
}

void ZBuffer::drawPolygonWireFrame(const Poly &polygon, const Attr &attr)
{
	auto &vertices = polygon.getVertices();
	Pixel first_pixel = {};
	Pixel last_pixel = {};

	for (int i = 0; i < vertices.size() - 1; i++)
	{
		auto px1 = toPixel(*vertices[i], attr);
		auto px2 = toPixel(*vertices[i + 1], attr);
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

void ZBuffer::drawPolygonSolid(const Poly &polygon, const Attr &attr)
{
	auto &vertices = polygon.getVertices();
	
	auto start = toPixel(*vertices[0], attr);
	auto target1 = toPixel(*vertices[1], attr);
	auto target2 = toPixel(*vertices[2], attr);
	auto curr1 = start;
	auto curr2 = start;

	while(!(curr1.x == target1.x && curr1.y == target1.y) &&
		!(curr2.x == target2.x && curr2.y == target2.y))
	{
		curr1 = nextPixel(start, target1, curr1);
		curr2 = nextPixel(start, target2, curr2);
		drawLine(curr1, curr2);
	}
}

void ZBuffer::draw(const Object &object, const Attr &attr)
{
	for (auto &mesh : object.getMeshes())
	{
		for (auto &polygon : mesh.getPolygons())
		{
			if (attr.drawing_mode == DrawingMode::WIRE_FRAME)
			{
				drawPolygonWireFrame(*polygon, attr);
			}
			else
			{
				drawPolygonSolid(*polygon, attr);
			}
		}
	}
}

void ZBuffer::drawLine(const Pixel &p1, const Pixel &p2)
{
	Pixel curr = p1;

	while (!(curr.x == p2.x && curr.y == p2.y))
	{
		curr = nextPixel(p1, p2, curr);
		set(curr);
	}
}

Pixel ZBuffer::toPixel(const Vertex &vertex, const Attr &attr)
{
	auto p = attr.T * vertex.pos.toHomogeneous();
	p /= p(3);

	int x_res = static_cast<uint>((_width / 2.0) * (p(0) + 1.0));
	int y_res = static_cast<uint>((_height / 2.0) * (1.0 - p(1)));

	return { x_res, y_res, p(2), vertex.color };
}

ZBuffer::~ZBuffer()
{
	if (!isEmpty())
	{
		delete[] _bits;
	}
}
