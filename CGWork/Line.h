#pragma once

#include "afxwin.h"
#include "Drawable.h"
#include "ZBuffer.h"

class Line : public Drawable
{
public:
	Line(const std::shared_ptr<Vertex> &from, const std::shared_ptr<Vertex> &to, const COLORREF &color);
	virtual void draw(ZBuffer &zbuffer, const Attr &attr) override;
	static void midPoint(ZBuffer &zbuffer, const Attr &attr, const Pixel &from, const Pixel &to, const Color &color);
	static double interpolateDepth(const int &x, const int &y, const Pixel &from, const Pixel &to);
	static Vec3d interpolateNormal(const int &x, const int &y, const Pixel &from, const Pixel &to);
	static Vec3d interpolatePos(const int &x, const int &y, const Pixel &from, const Pixel &to);
	int getMinYPixel()const { return _min_y_px; }
	int getMaxYPixel()const { return _max_y_px; }
	~Line();

private:
	Line &boundPixels(Pixel &from, Pixel &to, const int &width, const int &height);
	Line &swapPixels(Pixel &p1, Pixel &p2);
	static int calcX(const int &x, const int &y, const Pixel &from, const Pixel &to);
	static int calcY(const int &x, const int &y, const Pixel &from, const Pixel &to);
	static Vec2i initFrom(const Pixel &from, const Pixel &to);
	static Vec2i initTo(const Pixel &from, const Pixel &to);

public: //members
	std::shared_ptr<Vertex> _from;
	std::shared_ptr<Vertex> _to;
	COLORREF _color;
	int _min_y_px;
	int _max_y_px;
	Pixel _from_px;
	Pixel _to_px;
	bool _is_init_pixels;
};

