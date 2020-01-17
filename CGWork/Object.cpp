#include "Object.h"



Object::Object( CString str) : 
	_model(),
	_name(str),
	_pos(),
	_bounding_box({})
{

}

void Object::addMesh(const Mesh &mesh)
{
	_meshes.push_back(mesh);


	if (_meshes.size() == 1)
	{
		 _bounding_box.max_x = mesh.getBoundingBox().max_x;
		 _bounding_box.max_y = mesh.getBoundingBox().max_y;
		 _bounding_box.max_z = mesh.getBoundingBox().max_z;
		 _bounding_box.min_x = mesh.getBoundingBox().min_x;
		 _bounding_box.min_y = mesh.getBoundingBox().min_y;
		 _bounding_box.min_z = mesh.getBoundingBox().min_z;

		//set colors
		_fNormalColor = _vNormalColor = _BBColor = _meshes[0].getColor();
	}

	if (mesh.getBoundingBox().min_x < _bounding_box.min_x)
	{
		_bounding_box.min_x = mesh.getBoundingBox().min_x;
	}
	if (mesh.getBoundingBox().max_x > _bounding_box.max_x)
	{
		_bounding_box.max_x = mesh.getBoundingBox().max_x;
	}
	if (mesh.getBoundingBox().min_y < _bounding_box.min_y)
	{
		_bounding_box.min_y = mesh.getBoundingBox().min_y;
	}
	if (mesh.getBoundingBox().max_y > _bounding_box.max_y)
	{
		_bounding_box.max_y = mesh.getBoundingBox().max_y;
	}
	if (mesh.getBoundingBox().min_z < _bounding_box.min_z)
	{
		_bounding_box.min_z = mesh.getBoundingBox().min_z;
	}
	if (mesh.getBoundingBox().max_z > _bounding_box.max_z)
	{
		_bounding_box.max_z = mesh.getBoundingBox().max_z;
	}

	_pos = Vec3d((_bounding_box.min_x + _bounding_box.max_x) / 2, 
		(_bounding_box.min_y + _bounding_box.max_y) / 2, 
		(_bounding_box.min_z + _bounding_box.max_z) / 2);
}

Object & Object::rotateX(const double angle_deg)
{
	auto translation = _pos;
	_model.translate(-translation).rotateX(angle_deg).translate(translation);

	return *this;
}

Object & Object::rotateY(const double angle_deg)
{
	auto translation = _pos;
	_model.translate(-translation).rotateY(angle_deg).translate(translation);

	return *this;
}

Object & Object::rotateZ(const double angle_deg)
{
	auto translation = _pos;
	_model.translate(-translation).rotateZ(angle_deg).translate(translation);

	return *this;
}

Object & Object::translate(const Vec3d & translation)
{
	_model.translate(translation);
	_pos = _model.getTranslation();

	return *this;
}

Object & Object::scale(const Vec3d & scale)
{
	_model.scale(scale);

	return *this;
}

const Tmatd Object::getModel() const
{
	return _model;
}

std::vector<std::pair<Vec4d, Vec4d>> Object::getBoundingBoxLines()
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

//Object &Object::drawBoudingBox(ZBuffer &zbuffer, const Attr &attr)
//{
//	auto T = attr.T;
//	int width = zbuffer.getWidth();
//	int height = zbuffer.getHeight();
//	int *bits = zbuffer.getBits();
//
//	auto bb_lines = getBoundingBoxLines();
//	for (auto pair : bb_lines) {
//		Vec4d p1 = T * pair.first;
//		Vec4d p2;
//
//		p2 = T * pair.second;
//		p1 /= p1(3);
//		p2 /= p2(3);
//		auto px1 = coordsToPixels(p1(0), p1(1), width, height);
//		auto px2 = coordsToPixels(p2(0), p2(1), width, height);
//
//		MidPointDraw(px1(0), px1(1), px2(0), px2(1), bits, RGBToBGR(_BBColor),
//			width, height);
//	}
//
//	return *this;
//}

Object& Object::setVNColor(const COLORREF &color)
{
	_vNormalColor = color;
	for (auto &mesh : _meshes)
	{
		mesh.setVerticesNoramlsColor(color);
	}

	return *this;
}

Object& Object::setFNColor(const COLORREF &color)
{
	_fNormalColor = color;
	for (auto &mesh : _meshes)
	{
		mesh.setFacesNormalsColor(color);
	}

	return *this;
}

//void Object::draw(ZBuffer &zbuffer, const Attr &attr)
//{
//	for (auto &mesh : _meshes)
//	{
//		mesh.draw(zbuffer, attr);
//	}
//
//	if (attr.bounding_box) 
//	{
//		drawBoudingBox(zbuffer, attr);
//	}
//}

