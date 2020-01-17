#ifndef MATRICES_H
#define MATRICES_H

#include "Matrix.h"
#include "Vector.h"
#include "TranformationMatrix.h"


namespace consts
{
	const double PI = 3.14159265358979323846;
}

using Mat2d = Matrix<double, 2, 2>;
using Mat3d = Matrix<double, 3, 3>;
using Mat2f = Matrix<float, 2, 2>;
using Mat3f = Matrix<float, 3, 3>;
using Tmatd = TransformationMatrix<double>;
using Tmatf = TransformationMatrix<float>;

using Vec2d = Vector<double, 2>;
using Vec2u = Vector<uint, 2>;
using Vec2i = Vector<int, 2>;
using Vec3d = Vector<double, 3>;
using Vec4d = Vector<double, 4>;
using Vec2f = Vector<float, 2>;
using Vec3f = Vector<float, 3>;

#endif // MATRICES_H
