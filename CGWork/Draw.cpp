
#include "Draw.h"


bool isOutOfBound(int x, int y, int w, int h) 
{

	return x < 0 || y < 0 || x >= w || y >= h;
}

//assumes x1 < x2, y1 < y2,  0 < dy <= dx , assume x1 and y1 are inBound
void basicMidPointDraw1(int x1, int y1, int x2, int y2, int* bits, COLORREF color, int w, int h) 
{
	int firstX = -1, firstY = -1;
	int dx = x2 - x1, dy = y2 - y1;
	int x, y, d, E, NE;
	x = x1;
	y = y1;
	d = 2 * dy - dx;
	E = 2 * dy;
	NE = 2 * (dy - dx);


	if (!isOutOfBound(x, y, w, h))
	{
		bits[x + w *y] = color;
		firstX = x;
		firstY = y;
	}

	while (x < x2) 
	{
		if (d < 0) 
		{
			d += E;
			++x;
		}
		else 
		{
			d += NE;
			++x;
			++y;
		}
		if (!isOutOfBound(x, y, w, h))
		{
			if (firstX > -1 && firstY > -1)
			{
				bits[x + w * y] = color;
			}	
			else
			{
				bits[x + w * y] = color;
				firstX = x;
				firstY = y;
			}
		}
	}
}

//assumes x1 < x2, y1 < y2,           dy  > dx  >= 0 , assume x1 and y1 are inBound
void basicMidPointDraw2(int x1, int y1, int x2, int y2, int* bits, COLORREF color, int w, int h) 
{
	int firstX = -1, firstY = -1;
	int dx = x2 - x1, dy = y2 - y1;
	int x, y, d, N, NE;
	x = x1;
	y = y1;
	d = 2 * dx - dy;
	N = 2 * dx;
	NE = 2 * (dx - dy);


	if (!isOutOfBound(x, y, w, h))
	{
		bits[x + w * y] = color;
		firstX = x;
		firstY = y;
	}


	while (y < y2) 
	{
		if (d < 0) 
		{
			d += N;
			++y;
		}
		else 
		{
			d += NE;
			++x;
			++y;
		}

		if (!isOutOfBound(x, y, w, h))
		{
			if (firstX > -1 && firstY > -1)
			{
				bits[x + w * y] = color;
			}
						
			else
			{
				bits[x + w * y] = color;
				firstX = x;
				firstY = y;
			}
		}
	}
}




//assumes x1 < x2, y1 > y2,       -dx < dy < 0 < dx  , assume x1 and y1 are inBound
void basicMidPointDraw3(int x1, int y1, int x2, int y2, int* bits, COLORREF color, int w, int h) 
{
	int firstX = -1, firstY = -1;
	int dx = x2 - x1, dy = y2 - y1;
	int x, y, d, E, SE;
	x = x1;
	y = y1;

	d = 2 * dy - dx;//-200
	E = 2 * dy;//-100 this should be positive i guess
	SE = 2 * (dy + dx);//100 and this should be negative


	if (!isOutOfBound(x, y, w, h))
	{
		bits[x + w * y] = color;
		firstX = x;
		firstY = y;
	}



	while (x < x2) 
	{
		if (d > 0) 
		{
			d += E;
			++x;
		}
		else 
		{
			d += SE;
			++x;
			--y;
		}
		if (!isOutOfBound(x, y, w, h))
		{
			if (firstX > -1 && firstY > -1)
			{
				bits[x + w * y] = color;
			}
			else
			{
				bits[x + w * y] = color;
				firstX = x;
				firstY = y;
			}
		}
	}
}


//assumes x1 < x2, y1 > y2,           dy < -dx < 0 < dx < -dy , assume x1 and y1 are inBound
void basicMidPointDraw4(int x1, int y1, int x2, int y2, int* bits, COLORREF color, int w, int h) 
{
	int firstX = -1, firstY = -1;
	int dx = x2 - x1, dy = y2 - y1;
	int x, y, d, S, SE;

	x = x1;
	y = y1;

	d = 2 * dy - dx;
	S = 2 * dx;
	SE = 2 * (dy + dx);

	if (!isOutOfBound(x, y, w, h))
	{
		bits[x + w * y] = color;
		firstX = x;
		firstY = y;
	}

	while (y > y2) 
	{
		if (d < 0) 
		{
			d += S;
			--y;
		}
		else 
		{
			d += SE;
			++x;
			--y;
		}	

		if (!isOutOfBound(x, y, w, h))
		{
			if (firstX > -1 && firstY > -1)
			{
				bits[x + w * y] = color;
			}
			else
			{
				bits[x + w * y] = color;
				firstX = x;
				firstY = y;
			}
		}
	}
}

//color must be in BGR format not RGB
void MidPointDraw(int x1, int y1, int x2, int y2, int* bits, COLORREF color, int width, int height) 
{
	if (x2 < x1) // make sure that x1 <= x2, the == situation is ok
	{
		MidPointDraw(x2, y2, x1, y1, bits, color, width, height);
		return;
	}
	//now  x1 <= x2
	if ((x1 < 0 && x2 < 0) || (y1 < 0 && y2 < 0) || 
		(x1 > width && x2 > width) || (y1 > height && y2 > height))
	{
		return;
	}
		
	//take care of cases the are going out of screen bound
	double dx = x2 - x1, dy = y2 - y1;
	if (x1 < 0)
	{
		if (dx != 0)
		{
			MidPointDraw(0, y2 - (x2 * (dy / dx)), x2, y2, bits, color, width, height);
		}	

		return;
	}

	if (y1 < 0) 
	{
		if (dy != 0)
		{
			MidPointDraw(x2 - (y2*(dx / dy)), 0, x2, y2, bits, color, width, height);
		}	

		return;
	}

	if (y1 > height - 1) 
	{
		if (dy != 0)
		{
			MidPointDraw(x2 + (height - y2 - 1) * (dx / dy), height - 1, x2, y2, bits, 
				color, width, height);
		}		

		return;
	}
	
	if (y1 <= y2) 
	{
		if (y2 - y1 <= x2 - x1)
		{
			basicMidPointDraw1(x1, y1, x2, y2, bits, color, width, height);
		}
		else
		{
			basicMidPointDraw2(x1, y1, x2, y2, bits, color, width, height);
		}

		return;
	}

	//now  x1 <= x2 && y1 > y2

	if (x1 - x2 < y2 - y1)
	{
		basicMidPointDraw3(x1, y1, x2, y2, bits, color, width, height); //-dx < dy < 0 < dx
	}	
	else
	{
		basicMidPointDraw4(x1, y1, x2, y2, bits, color, width, height); //dy < -dx < 0 < dx < -dy
	}
		
	return;
}

COLORREF RGBToBGR(COLORREF col) 
{
	return RGB(GetBValue(col), GetGValue(col), GetRValue(col));
}

Vec2u coordsToPixels(const double &x, const double &y, const uint &width, const uint &height)
{
	double width_d = static_cast<double>(width);
	double height_d = static_cast<double>(height);

	uint x_res = static_cast<uint>((width_d / 2.0) * (x + 1.0));
	uint y_res = static_cast<uint>((height_d / 2.0) * (1.0 - y));

	return Vec2u(x_res, y_res);
}




