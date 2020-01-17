#include "Drawable.h"
#include "Types.h"


//Pixel Drawable::toPixels(const ZBuffer &zbuffer, const Attr &attr, const Vertex &point, const Color &color)
//{
//	auto p = attr.T * point.pos.toHomogeneous();
//	p /= p(3);
//
//	double width_d = static_cast<double>(zbuffer.getWidth());
//	double height_d = static_cast<double>(zbuffer.getHeight());
//
//	int x_res = static_cast<uint>((width_d / 2.0) * (p(0) + 1.0));
//	int y_res = static_cast<uint>((height_d / 2.0) * (1.0 - p(1)));
//
//	return { x_res, y_res, p(2), point.calc_normal, Vec3d(p(0), p(1), p(2)), color };
//}

