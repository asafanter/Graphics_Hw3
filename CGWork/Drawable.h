#pragma once

#include "Matrices.h"
#include "ZBuffer.h"
#include "Types.h"

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

class Drawable
{
public:
	virtual void draw(ZBuffer &zbuffer, const Attr &attr) = 0;
	static Pixel toPixels(const ZBuffer &zbuffer, const Attr &attr, const Vertex &point, const Color &color);
};

