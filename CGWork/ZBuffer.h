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
	Color RGBToBGR(COLORREF col);
	Pixel nextPixel(const Pixel &p1, const Pixel &p2, const Pixel &p);
	Pixel nextPixelFill(const Pixel &start, const Pixel &target, const Pixel &p);
	int calcQuarter(const Pixel &p2, const Pixel &p, const double &m);
	void drawLine(const Pixel &p1, const Pixel &p2);
	Pixel toPixel(const Vertex &vertex, const Attr &attr);
	Pixel interpolatePixel(const Pixel &p1, const Pixel &p2, const Pixel &p);
	void drawPolygonWireFrame(const Poly &polygon, const Attr &attr);
	void drawPolygonSolid(const Poly &polygon, const Attr &attr);
	void calcDrawOrder(Pixel &start, Pixel &via, Pixel &target, 
		const Poly &polygon, const Attr &Attr);

private: //members
	int *_bits;
	std::vector<double> _z;
	std::vector<bool> _drawn;
	uint _width;
	uint _height;
	Color _color;
	std::vector<LightParams> _lights;
};

