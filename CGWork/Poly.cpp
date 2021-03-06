#include "Poly.h"
#include "Line.h"

Poly::Poly() :
	_vertices(),
	_fnormal(),
	_fGivenNormal(),
	_color(RGB(0, 0, 0)),
	_faces_normals_color(RGB(255, 255, 255)),
	_min_y_pixel(-1),
	_max_y_pixel(-1),
	_bounding_box({}),
	_pos()
{

}

Poly &Poly::addVertex(const std::shared_ptr<Vertex> &vertex)
{
	_vertices.emplace_back(vertex);
	_pos *= _vertices.size() - 1;
	_pos += vertex->pos;
	_pos /= _vertices.size();
	
	if (_vertices.size() == 1)//first vertex
	{
		_bounding_box.min_x = _bounding_box.max_x = vertex->pos(0);
		_bounding_box.min_y = _bounding_box.max_y = vertex->pos(1);
		_bounding_box.min_z = _bounding_box.max_z = vertex->pos(2);

		return *this;
	}

	if (vertex->pos(0) > _bounding_box.max_x)
		_bounding_box.max_x = vertex->pos(0);
	if (vertex->pos(1) > _bounding_box.max_y)
		_bounding_box.max_y = vertex->pos(1);
	if (vertex->pos(2) > _bounding_box.max_z)
		_bounding_box.max_z = vertex->pos(2);

	if (vertex->pos(0) < _bounding_box.min_x)
		_bounding_box.min_x = vertex->pos(0);
	if (vertex->pos(1) < _bounding_box.min_y)
		_bounding_box.min_y = vertex->pos(1);
	if (vertex->pos(2) < _bounding_box.min_z)
		_bounding_box.min_z = vertex->pos(2);

	return *this;
}

void Poly::flipNormal()
{
	_fnormal = -_fnormal;
}

void Poly::SetGivenFaceNormal(double x, double y, double z)
{
	_fGivenNormal = Vec3d(x, y, z);
	_fGivenNormal.normalize();
}


void Poly::CalcFaceNormal()
{
	Vec3d p1(_vertices[0]->pos(0), _vertices[0]->pos(1), _vertices[0]->pos(2));
	Vec3d p2(_vertices[1]->pos(0), _vertices[1]->pos(1), _vertices[1]->pos(2));
	Vec3d p3(_vertices[2]->pos(0), _vertices[2]->pos(1), _vertices[2]->pos(2));

	Vec3d u = p3 - p1;
	Vec3d v = p2 - p1;

	Vec3d res = v.cross(u);
	res.normalize();
	_fnormal = res;
}

std::vector<std::shared_ptr<Poly>> Poly::createPolygons3Vertices(const Poly &polygon)
{
	std::vector<std::shared_ptr<Poly>> res;

	auto &vertices = polygon.getVertices();

	if (vertices.size() == 3)
	{
		auto p = std::make_shared<Poly>(polygon);
		res.push_back(p);
	}
	else
	{
		auto polygon1 = std::make_shared<Poly>();
		auto polygon2 = std::make_shared<Poly>();

		polygon1->addVertex(vertices[0]);
		polygon1->addVertex(vertices[1]);
		polygon1->addVertex(vertices[2]);

		polygon2->addVertex(vertices[2]);
		polygon2->addVertex(vertices[3]);
		polygon2->addVertex(vertices[0]);

		polygon1->setColor(polygon.getColor());
		polygon1->setNormal(polygon.getCalcFaceNormal());

		polygon2->setColor(polygon.getColor());
		polygon2->setNormal(polygon.getCalcFaceNormal());

		polygon1->setPos(polygon.getPos());
		polygon2->setPos(polygon.getPos());

		res.push_back(polygon1);
		res.push_back(polygon2);
	}

	return res;
}

//Poly &Poly::fillWithColor(ZBuffer &zbuffer, const Attr &attr)
//{
//	std::vector<Pixel> pixels;
//	for (int i = _min_y_pixel; i <= _max_y_pixel; i++)
//	{
//		pixels.clear();
//
//		for (auto &line : _lines)
//		{
//			if (i <= line._max_y_px && i >= line._min_y_px)
//			{
//				auto p1 = line._from_px;
//				auto p2 = line._to_px;
//
//				auto dy = p2.y - p1.y;
//				auto dx = p2.x - p1.x;
//
//				if (dy == 0)
//				{
//					continue;
//				}
//
//				int x = (i - p2.y) * dx / dy + p2.x;
//				int y = i;
//				auto depth = Line::interpolateDepth(x, y, p1, p2);
//				auto normal = Line::interpolateNormal(x, y, p1, p2);
//				auto pos = Line::interpolatePos(x, y, p1, p2);
//
//				Pixel pixel = { x, y, depth, normal, pos, _color};
//
//				auto it = std::find(pixels.begin(), pixels.end(), pixel);
//
//				if (it == pixels.end())
//				{
//					pixels.push_back(pixel);
//				}
//			}
//		}
//		if (pixels.size() == 2)
//		{
//			Line::midPoint(zbuffer, attr, pixels[0], pixels[1], _color);
//		}
//	}
//
//	return *this;
//}

//void Poly::draw(ZBuffer &zbuffer, const Attr &attr)
//{
//	drawWireFrame(zbuffer, attr);
//	findMinMaxYPixels();
//
//	//fillWithColor(zbuffer, attr);
//
//	if (attr.face_normals)
//	{
//		drawFaceNormals(zbuffer, attr);
//	}
//}

//Poly &Poly::drawFaceNormals(ZBuffer &zbuffer, const Attr &attr)
//{
//	auto &T = attr.T;
//	int width = zbuffer.getWidth();
//	int height = zbuffer.getHeight();
//	int *bits = zbuffer.getBits();
//	Vertex p1 = {};
//	Vertex p2 = {};
//
//	p1.pos = _pos;
//	Vec3d normal;
//	if (attr.given_face_normals && getGivenFaceNormal()(3) == 1)
//		normal = getGivenFaceNormal();
//	else
//		normal = getCalcFaceNormal();
//
//	p2.pos = p1.pos + normal;
//
//	auto px1 = Drawable::toPixels(zbuffer, attr, p1, _faces_normals_color);
//	auto px2 = Drawable::toPixels(zbuffer, attr, p2, _faces_normals_color);
//
//	MidPointDraw(px1.x, px1.y, px2.x, px2.y, bits, RGBToBGR(_faces_normals_color),
//		width, height);
//
//	return *this;
//}

//void Poly::drawWireFrame(ZBuffer &zbuffer, const Attr &attr)
//{
//	for (auto &line : _lines)
//	{
//		line.draw(zbuffer, attr);
//	}
//}



