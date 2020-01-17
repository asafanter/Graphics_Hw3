#pragma once

#include "Matrices.h"
#include "ZBuffer.h"
#include "Types.h"

class Drawable
{
public:
	virtual void draw(ZBuffer &zbuffer, const Attr &attr) = 0;
	//static Pixel toPixels(const ZBuffer &zbuffer, const Attr &attr, const Vertex &point, const Color &color);
};

