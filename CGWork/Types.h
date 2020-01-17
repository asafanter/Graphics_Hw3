#pragma once

#include <afxwin.h>

#include "Matrices.h"

using Color = COLORREF;

class Poly;

struct Pixel
{
	int x;
	int y;
	double depth;
	Vec3d normal;
	Vec3d pos;
	Color color;

	bool operator==(const Pixel &p)
	{
		return x == p.x && y == p.y && depth == p.depth && color == p.color;
	}
};

struct Vertex
{
	Vec3d pos;
	Vec3d given_normal;
	Vec3d calc_normal;
	std::vector<std::shared_ptr<Poly>> neighbors_polygons;
};

struct BoundingBox
{
	double min_x;
	double max_x;
	double min_y;
	double max_y;
	double min_z;
	double max_z;
};

enum LightMethod
{
	FLAT,
	GOURAUD,
	PHONG
};

struct Attr
{
	bool face_normals;
	bool vertices_normals;
	bool given_face_normals;
	bool given_vertices_normals;
	bool bounding_box;
	Tmatd T;
	double near_plane;
	LightMethod light_method;
};