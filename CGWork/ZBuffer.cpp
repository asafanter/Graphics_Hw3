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

void ZBuffer::draw(const Object &object, const Attr &attr)
{

}

ZBuffer::~ZBuffer()
{
	if (!isEmpty())
	{
		delete[] _bits;
	}
}
