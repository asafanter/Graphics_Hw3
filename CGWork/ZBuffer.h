#pragma once

#include <afxwin.h>

#include "Matrices.h"
#include "Light.h"
#include "Types.h"
#include "Object.h"

class ZBuffer
{
public:
	ZBuffer();
	ZBuffer(const uint &width, const uint &height, const Color &color = RGB(0, 0, 0));
	ZBuffer &resize(const uint &width, const uint &height);
	ZBuffer &set(const Pixel &p);
	ZBuffer &setDefaultColor(const Color &color);
	Color getColor(const uint &x, const uint &y) const { return _bits[x + _width * y]; }
	uint getWidth() const { return _width; }
	uint getHeight() const { return _height; }
	bool isEmpty() const { return _bits == nullptr; }
	int *getBits() const { return _bits; }
	void draw(const Object &object, const Attr &attr);
	~ZBuffer();

private: //defs
	static constexpr uint MAX_LIGHTS = 7;

public: //methods
	static Color RGBToBGR(COLORREF col);
	static Vec2i nextPixel(const Vec2i &p1, const Vec2i &p2, const Vec2i &p);
	static int calcQuarter(const Vec2i &p2, const Vec2i &p, const double &m);

private: //members
	int *_bits;
	std::vector<double> _z;
	std::vector<bool> _drawn;
	uint _width;
	uint _height;
	Color _color;
	std::vector<LightParams> _lights;
};

