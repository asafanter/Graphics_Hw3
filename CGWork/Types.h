#pragma once

#include <afxwin.h>

#include "Matrices.h"

using Color = COLORREF;

class Poly;

struct Vec3
{
	double x;
	double y;
	double z;

	Vec3 &normalize()
	{
		double norm = sqrt(x * x + y * y + z * z);
		if (norm == 0)
		{
			return *this;
		}

		x /= norm;
		y /= norm;
		z /= norm;

		return *this;
	}
	Vec3 operator - ()
	{
		return { -x ,-y, -z };
	}
	Vec3 operator+(const Vec3 &other) const
	{
		return { x + other.x, y + other.y, z + other.z };
	}
	Vec3 operator-(const Vec3 &other) const
	{
		return { x - other.x, y - other.y, z - other.z };
	}
	Vec3 operator*(const double &scalar) const
	{
		return { x * scalar, y * scalar, z * scalar };
	}
	Vec3 operator/(const double &scalar)
	{
		return { x / scalar, y / scalar, z / scalar };
	}
	double dot(const Vec3 &other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}
	Vec3 elementMultiply(const Vec3 &other)
	{
		return { x * other.x, y * other.y, z * other.z };
	}
};

struct Pixel
{
	int x;
	int y;
	double depth;
	Color color;
	Vec3 normal;
	Vec3 pos;

	bool operator==(const Pixel &p)
	{
		return x == p.x && y == p.y;
	}
	bool operator!=(const Pixel &p)
	{
		return !(*this == p);
	}
};

struct Vertex
{
	Vec3d pos;
	Vec3d given_normal;
	Vec3d calc_normal;
	Color color;
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

enum Shading
{
	FLAT,
	GOURAUD,
	PHONG
};

enum DrawingMode
{
	WIRE_FRAME,
	SOLID
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
	Shading shading;
	DrawingMode drawing_mode;
	Vec3d view_pos;
	bool back_face_culling;
};