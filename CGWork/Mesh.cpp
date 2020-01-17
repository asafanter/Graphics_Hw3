#include "Mesh.h"
#include <algorithm>
#include <string.h>

//init all colors to same color
Mesh::Mesh(COLORREF col, char* str) :
	_polygons(),
	_vertices(),
	_pos(0.0, 0.0, 0.0),
	_model(),
	_color(col),
	_name(str),
	_vertices_normals_color(RGB(255, 255, 255)),
	_faces_normals_color(RGB(255, 255, 255)),
	_bounding_box({})
{
	

}

Mesh &Mesh::setFacesNormalsColor(const Color &new_color)
{
	_faces_normals_color = new_color;
	for (auto &polygon : _polygons)
	{
		polygon->setFacesNormalsColor(new_color);
	}

	return *this;
}

Mesh & Mesh::calcVNormals()
{
	for (auto &polygon : _polygons)
	{
		for (auto &vertex : polygon->getVertices())
		{
			Vec3d avg;
			for (auto &neighbor : vertex->neighbors_polygons)
			{
				Vec3d neighbor_normal(neighbor->getCalcFaceNormal()(0), neighbor->getCalcFaceNormal()(1), 
				neighbor->getCalcFaceNormal()(2));
				avg += neighbor_normal;
			}
			avg /= vertex->neighbors_polygons.size();
			avg.normalize();
			vertex->calc_normal = avg;
		}
	}


	return *this;
}

Mesh &Mesh::addPolygon(std::shared_ptr<Poly> &new_polygon)
{
	for (auto &vertex : new_polygon->getVertices())
	{
		vertex->neighbors_polygons.push_back(new_polygon);
		for (auto &exist_polygon : _polygons)
		{
			for (auto &exist_vertex : exist_polygon->getVertices())
			{
				if (exist_vertex->pos == vertex->pos) 
				{
					exist_vertex->neighbors_polygons.push_back(new_polygon);
					vertex->neighbors_polygons.push_back(exist_polygon);
				}
			}
		}
		
		bool has_found = false;
		for (auto &exist_vertex : _vertices)
		{
			if (exist_vertex->pos == vertex->pos)
			{
				has_found = true;
			}
		}
		if (!has_found)
		{
			_vertices.push_back(vertex);
		}
	}

	_polygons.push_back(new_polygon);

	if (_polygons.size() == 1)
	{
		_bounding_box.min_x = new_polygon->getBoundingBox().min_x;
		_bounding_box.max_x = new_polygon->getBoundingBox().max_x;
		_bounding_box.min_y = new_polygon->getBoundingBox().min_y;
		_bounding_box.max_y = new_polygon->getBoundingBox().max_y;
		_bounding_box.min_z = new_polygon->getBoundingBox().min_z;
		_bounding_box.max_z = new_polygon->getBoundingBox().max_z;

		return *this;
	}

	if (new_polygon->getBoundingBox().min_x < _bounding_box.min_x)
	{
		_bounding_box.min_x = new_polygon->getBoundingBox().min_x;
	}
	if (new_polygon->getBoundingBox().max_x > _bounding_box.max_x)
	{
		_bounding_box.max_x = new_polygon->getBoundingBox().max_x;
	}
	if (new_polygon->getBoundingBox().min_y < _bounding_box.min_y)
	{
		_bounding_box.min_y = new_polygon->getBoundingBox().min_y;
	}
	if (new_polygon->getBoundingBox().max_y > _bounding_box.max_y)
	{
		_bounding_box.max_y = new_polygon->getBoundingBox().max_y;
	}
	if (new_polygon->getBoundingBox().min_z < _bounding_box.min_z)
	{
		_bounding_box.min_z = new_polygon->getBoundingBox().min_z;
	}
	if (new_polygon->getBoundingBox().max_z > _bounding_box.max_z)
	{
		_bounding_box.max_z = new_polygon->getBoundingBox().max_z;
	}

	return *this;
}

std::shared_ptr<Vertex> Mesh::findVertex(const std::shared_ptr<Vertex> &vertex)
{
	for (auto &v : _vertices)
	{
		if (v->pos == vertex->pos)
		{
			return v;
		}
	}

	return nullptr;
}

const Tmatd Mesh::getModel() const
{
	return _model;
}


std::vector<std::pair<Vec4d, Vec4d>> Mesh::getBoundingBoxLines()
{
	Vec4d p1(_bounding_box.max_x, _bounding_box.max_y, _bounding_box.min_z, 1.0);
	Vec4d p2(_bounding_box.max_x, _bounding_box.max_y, _bounding_box.max_z, 1.0);
	Vec4d p3(_bounding_box.min_x, _bounding_box.max_y, _bounding_box.min_z, 1.0);
	Vec4d p4(_bounding_box.min_x, _bounding_box.max_y, _bounding_box.max_z, 1.0);
	Vec4d p5(_bounding_box.max_x, _bounding_box.min_y, _bounding_box.max_z, 1.0);
	Vec4d p6(_bounding_box.min_x, _bounding_box.min_y, _bounding_box.max_z, 1.0);
	Vec4d p7(_bounding_box.min_x, _bounding_box.min_y, _bounding_box.min_z, 1.0);
	Vec4d p8(_bounding_box.max_x, _bounding_box.min_y, _bounding_box.min_z, 1.0);


	std::pair<Vec4d, Vec4d> l1(p1, p2), l2(p2, p3), l3(p3, p4), l4(p4, p1), l5(p5, p6), l6(p6, p7), l7(p7, p8), l8(p8, p5), l9(p1, p5), l10(p2, p8), l11(p3, p7), l12(p4, p6);

	std::vector<std::pair<Vec4d, Vec4d>> container;

	container.push_back(l1);
	container.push_back(l2);
	container.push_back(l3);
	container.push_back(l4);
	container.push_back(l5);
	container.push_back(l6);
	container.push_back(l7);
	container.push_back(l8);
	container.push_back(l9);
	container.push_back(l10);
	container.push_back(l11);
	container.push_back(l12);

	return container;
}


Mesh &Mesh::rotateX(const double angle_deg)
{
	_model.rotateX(angle_deg);

	return *this;
}

Mesh &Mesh::rotateY(const double angle_deg)
{
	_model.rotateY(angle_deg);

	return *this;
}

Mesh &Mesh::rotateZ(const double angle_deg)
{
	_model.rotateZ(angle_deg);

	return *this;
}

Mesh &Mesh::translate(const Vec3d &translation)
{
	_model.translate(translation);

	return *this;
}

Mesh &Mesh::scale(const Vec3d &scale)
{
	_model.scale(scale);

	return *this;
}

void Mesh::draw(ZBuffer &zbuffer, const Attr &attr)
{
	for (auto &polygon : _polygons)
	{
		polygon->setColor(_color);
		polygon->draw(zbuffer, attr);
	}

	if (attr.vertices_normals)
	{
		drawVerticesNormals(zbuffer, attr);
	}
}

Mesh &Mesh::drawVerticesNormals(ZBuffer &zbuffer, const Attr &attr)
{
	auto &T = attr.T;
	int width = zbuffer.getWidth();
	int height = zbuffer.getHeight();
	int *bits = zbuffer.getBits();
	Vertex p1 = {};
	Vertex p2 = {};

	for (auto &vertex : _vertices) {
		p1.pos = vertex->pos;
		Vec3d normal = attr.given_vertices_normals ? vertex->given_normal : vertex->calc_normal;
			
		p2.pos = p1.pos + normal;

		auto px1 = Drawable::toPixels(zbuffer, attr, p1, _vertices_normals_color);
		auto px2 = Drawable::toPixels(zbuffer, attr, p2, _vertices_normals_color);

		MidPointDraw(px1.x, px1.y, px2.x, px2.y, bits, RGBToBGR(_vertices_normals_color),
			width, height);
	}

	return *this;
}

