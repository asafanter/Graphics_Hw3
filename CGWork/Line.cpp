//#include <cmath>
//
//#include "Line.h"
//
//
//
//Line::Line(const std::shared_ptr<Vertex> &from, const std::shared_ptr<Vertex> &to, const COLORREF &color) :
//	_from(from),
//	_to(to),
//	_color(color),
//	_min_y_px(-1),
//	_max_y_px(-1),
//	_from_px(),
//	_to_px(),
//	_is_init_pixels(false)
//{
//
//}
//
////void Line::draw(ZBuffer &zbuffer, const Attr &attr) 
////{
////	auto from = Drawable::toPixels(zbuffer, attr, *_from, _color);
////	auto to = Drawable::toPixels(zbuffer, attr, *_to, _color);
////
////	if (!_is_init_pixels)
////	{
////		_from_px = from;
////		_to_px = to;
////		_is_init_pixels = true;
////	}
////
////	_min_y_px = min(from.y, to.y);
////	_max_y_px = max(from.y, to.y);
////
////	if ((from.x < 0 && to.x < 0) || 
////		(from.y < 0 && to.y < 0) ||
////		(from.x >= zbuffer.getWidth() && to.x >= zbuffer.getWidth()) ||
////		(from.y >= zbuffer.getHeight() && to.y >= zbuffer.getHeight()))
////	{
////		return;
////	}
////
////	boundPixels(from, to, zbuffer.getWidth(), zbuffer.getHeight());
////
////	midPoint(zbuffer, attr, from, to, _color);
////}
//
//double Line::interpolateDepth(const int &x, const int &y, const Pixel &from, const Pixel &to)
//{
//	double line_dist = std::sqrt(std::pow(from.x - to.x, 2) + std::pow(from.y - to.y, 2));
//	double dist_from_begin = std::sqrt(std::pow(x - from.x, 2) + std::pow(y - from.y, 2));
//
//	if (x == from.x && y == from.y)
//	{
//		return from.depth;
//	}
//
//	if (x == to.x && y == to.y)
//	{
//		return to.depth;
//	}
//
//	double a = dist_from_begin / line_dist;
//
//	return a * to.depth + (1 - a) * from.depth;
//}
//
//Vec3d Line::interpolateNormal(const int &x, const int &y, const Pixel &from, const Pixel &to)
//{
//	double line_dist = std::sqrt(std::pow(from.x - to.x, 2) + std::pow(from.y - to.y, 2));
//	double dist_from_begin = std::sqrt(std::pow(x - from.x, 2) + std::pow(y - from.y, 2));
//
//	if (x == from.x && y == from.y)
//	{
//		return from.normal;
//	}
//
//	if (x == to.x && y == to.y)
//	{
//		return to.normal;
//	}
//
//	double a = dist_from_begin / line_dist;
//
//	auto res = to.normal * a + from.normal * (1 - a);
//	res.normalize();
//
//	return res;
//}
//
//Vec3d Line::interpolatePos(const int &x, const int &y, const Pixel &from, const Pixel &to)
//{
//	double line_dist = std::sqrt(std::pow(from.x - to.x, 2) + std::pow(from.y - to.y, 2));
//	double dist_from_begin = std::sqrt(std::pow(x - from.x, 2) + std::pow(y - from.y, 2));
//
//	if (x == from.x && y == from.y)
//	{
//		return from.pos;
//	}
//
//	if (x == to.x && y == to.y)
//	{
//		return to.pos;
//	}
//
//	double a = dist_from_begin / line_dist;
//
//	auto res = to.pos * a + from.pos * (1 - a);
//
//	return res;
//}
//
////void Line::midPoint(ZBuffer &zbuffer, const Attr &attr, const Pixel &from, const Pixel &to, const Color &color)
////{
////	auto p1 = initFrom(from, to);
////	auto p2 = initTo(from, to);
////
////	int x1 = p1(0);
////	int y1 = p1(1);
////	int x2 = p2(0);
////	int y2 = p2(1);
////
////	int x = p1(0);
////	int y = p1(1);
////	int dy = y2 - y1;
////	int dx = x2 - x1;
////	int d = 2 * dy - dx;
////	int de = 2 * dy;
////	int dne = 2 * (dy - dx);
////
////	while (x <= x2)
////	{
////		auto new_x = calcX(x, y, from, to);
////		auto new_y = calcY(x, y, from, to);
////
////		//auto depth = interpolateDepth(new_x, new_y, from, to);
////		//auto normal = interpolateNormal(new_x, new_y, from, to);
////		//auto pos = interpolatePos(new_x, new_y, from, to);
////		zbuffer.set({ new_x, new_y, 0.0, Vec3d(), Vec3d(), color });
////
////		if (d < 0)
////		{
////			d += de;
////			x++;
////		}
////		else
////		{
////			d += dne;
////			x++;
////			y++;
////		}
////	}
////}
//
//Vec2i Line::initFrom(const Pixel &from, const Pixel &to)
//{
//	int x1 = from.x <= to.x ? from.x : to.x;
//	int y1 = from.x <= to.x ? from.y : to.y;
//	int x2 = from.x <= to.x ? to.x : from.x;
//	int y2 = from.x <= to.x ? to.y : from.y;
//
//	return Vec2i(x1, y1);
//}
//
//Vec2i Line::initTo(const Pixel &from, const Pixel &to)
//{
//	int x1 = from.x <= to.x ? from.x : to.x;
//	int y1 = from.x <= to.x ? from.y : to.y;
//	int x2 = from.x <= to.x ? to.x : from.x;
//	int y2 = from.x <= to.x ? to.y : from.y;
//	int dy = std::fabsl(y2 - y1);
//	int dx = std::fabsl(x2 - x1);
//
//	if (y1 <= y2)
//	{
//		if (dy <= dx)
//		{
//			return Vec2i(x2, y2);
//		}
//		else
//		{
//			return Vec2i(x1 + y2 - y1, y1 + x2 - x1);
//		}
//	}
//	else
//	{
//		if (dy <= dx)
//		{
//			return Vec2i(x2, y2 + 2 * (y1 - y2));
//		}
//		else
//		{
//			return Vec2i(x1 + y1 - y2, y1 + x2 - x1);
//		}
//	}
//}
//
//int Line::calcX(const int &x, const int &y, const Pixel &from, const Pixel &to)
//{
//	int x1 = from.x <= to.x ? from.x : to.x;
//	int y1 = from.x <= to.x ? from.y : to.y;
//	int x2 = from.x <= to.x ? to.x : from.x;
//	int y2 = from.x <= to.x ? to.y : from.y;
//	int dy = std::fabsl(y2 - y1);
//	int dx = std::fabsl(x2 - x1);
//
//	if (y1 <= y2)
//	{
//		if (dy <= dx)
//		{
//			return x;
//		}
//		else
//		{
//			return x1 + y - y1;
//		}
//	}
//	else
//	{
//		if (dy <= dx)
//		{
//			return x;
//		}
//		else
//		{
//			return x1 + y - y1;
//		}
//	}
//}
//
//int Line::calcY(const int &x, const int &y, const Pixel &from, const Pixel &to)
//{
//	int x1 = from.x <= to.x ? from.x : to.x;
//	int y1 = from.x <= to.x ? from.y : to.y;
//	int x2 = from.x <= to.x ? to.x : from.x;
//	int y2 = from.x <= to.x ? to.y : from.y;
//	int dy = std::fabsl(y2 - y1);
//	int dx = std::fabsl(x2 - x1);
//
//	if (y1 <= y2)
//	{
//		if (dy <= dx)
//		{
//			return y;
//		}
//		else
//		{
//			return y1 + x - x1;
//		}
//	}
//	else
//	{
//		if (dy <= dx)
//		{
//			return 2 * y1 - y;
//		}
//		else
//		{
//			return y1 - x + x1;
//		}
//	}
//}
//
//Line &Line::boundPixels(Pixel &from, Pixel &to, const int &width, const int &height)
//{
//	int dx = to.x - from.x;
//	int dy = to.y - from.y;
//
//	if (to.x < 0 || to.x >= width || to.y < 0 || to.y >= height)
//	{
//		swapPixels(from, to);
//	}
//
//	if (from.x < 0 && dx != 0)
//	{
//		from.y = to.y - to.x * dy / dx;
//		from.x = 0;
//	}
//
//	else if (from.x >= width && dx != 0)
//	{
//		from.y = to.y + (width - 1 - to.x) * dy / dx;
//		from.x = width - 1;
//	}
//
//	else if (from.y < 0 && dy != 0)
//	{
//		from.x = to.x - to.y * dx / dy;
//		from.y = 0;
//	}
//
//	else if (from.y >= height && dy != 0)
//	{
//		from.x = to.x + (height - 1 - to.y) * dx / dy;
//		from.y = height - 1;
//	}
//
//	return *this;
//}
//
//Line &Line::swapPixels(Pixel &p1, Pixel &p2)
//{
//	Pixel tmp = p1;
//	p1 = p2;
//	p2 = tmp;
//
//	return *this;
//}
//
//Line::~Line()
//{
//}
