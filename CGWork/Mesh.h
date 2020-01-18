#pragma once

#include <afxwin.h>

#include "Poly.h"

class Mesh
{
public:
	Mesh(COLORREF col, char* str);
	Mesh &calcVNormals();
	Mesh &addRawPolygon(std::shared_ptr<Poly> &new_polygon);
	const std::vector<std::shared_ptr<Poly>>& getPolygons() const { return _polygons; }
	const std::vector<std::shared_ptr<Poly>>& getRawPolygons() const { return _raw_polygons; }
	Mesh &rotateX(const double angle_deg);
	Mesh &rotateY(const double angle_deg);
	Mesh &rotateZ(const double angle_deg);
	Mesh &translate(const Vec3d &translation);
	Mesh &scale(const Vec3d &scale);
	const BoundingBox &getBoundingBox() const { return _bounding_box; }
	const Tmatd getModel() const;
	const COLORREF getColor() const { return _color; }
	Mesh& setColor(const COLORREF &color) { _color = color;  return *this;}
	Mesh &setVerticesNoramlsColor(const Color &new_color) { _vertices_normals_color = new_color; return *this; }
	Mesh &setFacesNormalsColor(const Color &new_color);
	Vec4d getPos() { return Vec4d(_pos(0), _pos(1), _pos(2), 1.0); }
	std::vector<std::pair<Vec4d, Vec4d>> getBoundingBoxLines();
	CString getName() { return _name; }
	~Mesh() = default;
	friend class Object;
	std::vector<std::shared_ptr<Poly>> _raw_polygons;
	std::vector<std::shared_ptr<Poly>> _polygons;

private: //methods
	//Mesh &drawVerticesNormals(ZBuffer &zbuffer, const Attr &attr);
	std::shared_ptr<Vertex> findVertex(const std::shared_ptr<Vertex> &vertex);

private: //members
	
	std::vector<std::shared_ptr<Vertex>> _vertices;
	Vec3d _pos;
	Tmatd _model;
	Color _color;
	CString _name;
	Color _vertices_normals_color;
	Color _faces_normals_color;
	BoundingBox _bounding_box;
};