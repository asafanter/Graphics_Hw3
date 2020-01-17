#pragma once

#include <vector>

#include "Matrices.h"
#include "Types.h"

class Poly 
{
public:
	Poly();
	Poly &addVertex(const std::shared_ptr<Vertex> &vertex);
	void SetGivenFaceNormal(double x, double y, double z);
	void CalcFaceNormal();
	const Vec3d getCalcFaceNormal() const { return _fnormal; }
	const Vec3d getGivenFaceNormal() const { return _fGivenNormal; }
	const BoundingBox &getBoundingBox() const { return _bounding_box; }
	const std::vector<std::shared_ptr<Vertex>> getVertices() const { return _vertices; }
	std::vector<std::shared_ptr<Vertex>> &getVertices() { return _vertices; }
	//void draw(ZBuffer &zbuffer, const Attr &attr) override;
	Color getColor() const { return _color; }
	Vec3d getPos() const { return _pos; }
	Poly &setColor(const Color &new_color) { _color = new_color; return *this; }
	Poly &setFacesNormalsColor(const Color &new_color) { _faces_normals_color = new_color; return *this; }
	~Poly() = default;
	friend class Mesh;

private: //methods
	//void drawWireFrame(ZBuffer &zbuffer, const Attr &attr);
	//Poly &drawFaceNormals(ZBuffer &zbuffer, const Attr &attr);
	//Poly &fillWithColor(ZBuffer &zbuffer, const Attr &attr);


private: //members
	std::vector<std::shared_ptr<Vertex>> _vertices;
	//std::vector<Line> _lines;
	Vec3d _fnormal;
	Vec3d _fGivenNormal;
	Color _color;
	Color _faces_normals_color;
	int _min_y_pixel;
	int _max_y_pixel;
	BoundingBox _bounding_box;
	Vec3d _pos;
};

