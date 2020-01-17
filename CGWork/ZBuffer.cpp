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

int ZBuffer::calcQuarter(const Vec2i &p2, const Vec2i &p, const double &m)
{
	if (std::abs(m) < 1.0)
	{
		if (m > 0)
		{
			if (p(0) <= p2(0))
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
			if (p(0) <= p2(0))
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
			if (p(0) <= p2(0))
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
			if (p(0) <= p2(0))
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

Vec2i ZBuffer::nextPixel(const Vec2i &p1, const Vec2i &p2, const Vec2i &p)
{
	int dy = p2(1) - p1(1);
	int dx = p2(0) - p1(0);
	int x = 1;
	int y = -1;

	if (p2(1) == p(1) && p2(0) > p(0))
	{
		return Vec2i(p(0) + 1, p(1));
	}
	if (p2(1) == p(1) && p2(0) < p(0))
	{
		return Vec2i(p(0) - 1, p(1));
	}
	if (p2(0) == p(0) && p2(1) > p(1))
	{
		return Vec2i(p(0), p(1) + 1);
	}
	if (p2(0) == p(0) && p2(1) < p(1))
	{
		return Vec2i(p(0), p(1) - 1);
	}

	double m = static_cast<double>(dy) / static_cast<double>(dx);

	int quarter = calcQuarter(p2, p, m);

	if (quarter == 1 || quarter == 3 || quarter == 5 || quarter == 7)
	{
		x = quarter < 4 ? p(0) + 1 : p(0) - 1;
		y = m * (x - p1(0)) + p1(1);
	}
	else
	{
		y = quarter == 2 || quarter == 6 ? p(1) + 1 : p(1) - 1;
		x = (1 / m) * (y + m * p1(0) - p1(1));
	}

	return Vec2i(x, y);
}

void ZBuffer::draw(const Object &object, const Attr &attr)
{
	for (auto &mesh : object.getMeshes())
	{
		for (auto &polygon : mesh.getPolygons())
		{
			auto &vertices = polygon->getVertices();
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
	}
}

void ZBuffer::drawLine(const Pixel &p1, const Pixel &p2)
{
	Vec2i target(p2.x, p2.y);
	Vec2i start(p1.x, p1.y);
	Vec2i curr = start;

	while (!(curr(0) == target(0) && curr(1) == target(1)))
	{
		curr = nextPixel(start, target, curr);
		Pixel px = { curr(0), curr(1), 1.0, p1.color };
		set(px);
	}
}

Pixel ZBuffer::toPixel(const Vertex &vertex, const Attr &attr)
{
	auto p = attr.T * vertex.pos.toHomogeneous();
	p /= p(3);

	int x_res = static_cast<uint>((_width / 2.0) * (p(0) + 1.0));
	int y_res = static_cast<uint>((_height / 2.0) * (1.0 - p(1)));

	return { x_res, y_res, p(2), RGB(255, 0, 0) };
}

ZBuffer::~ZBuffer()
{
	if (!isEmpty())
	{
		delete[] _bits;
	}
}
