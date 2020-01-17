#ifndef DRAW_H
#define DRAW_H

#include <afxwin.h>

#include "Matrices.h"
struct DrawingAttr
{
	int *bits;
	int width;
	int height;
	bool face_normals;
	bool vertices_normals;
	bool given_face_normals;
	bool given_vertices_normals;
	bool bounding_box;
	Tmatd projection;
	Tmatd look_at;
	Tmatd view;
	double near_plane;
};

void MidPointDraw(int x1, int y1, int x2, int y2, int* bits, COLORREF color, 
	int width, int height);
COLORREF RGBToBGR(COLORREF col);//used to switch the order because for some reason the bitmap reads colors in BGR format
Vec2u coordsToPixels(const double &x, const double &y, const uint &width, const uint &height);

#endif // DRAW_H
