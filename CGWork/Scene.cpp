#include "Scene.h"
#include "Line.h"
#include "Draw.h"

#define DEF_BOUNDING_BOX_SCREEN_RATION 3

Scene::Scene() :
	_objs(),
	_view(),
	_projection(),
	_is_initialized(false),
	_background_color(RGB(0, 0, 0)),
	_camera({}),
	_look_at(),
	_drawing_mode(DrawingMode::SOLID),
	_lights(),
	_is_normals_fliped(false)
{
	_projection = TransformationMatrix<double>::ortho(-10.0, 10.0, -5.0, 5.0, -5.0, 5.0);
	//_projection = TransformationMatrix<double>::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	_camera.pos = Vec3d(0.0, 0.0, 3.0);
	_camera.front = Vec3d(0.0, 0.0, -1.0);
	_camera.up = Vec3d(0.0, 1.0, 0.0);

	_look_at = lookAt(_camera.pos, _camera.pos + _camera.front, _camera.up);
}

void Scene::flipNormals()
{
	_is_normals_fliped = !_is_normals_fliped;

	for (auto &object : _objs)
	{
		object.flipNormals();
	}
}

Scene &Scene::reset()
{
	_objs.clear();
	_view = Tmatd();
	_projection = TransformationMatrix<double>::ortho(-10.0, 10.0, -5.0, 5.0, -5.0, 5.0);
	_is_initialized = false;
	_background_color = RGB(0, 0, 0);
	_camera.pos = Vec3d(0.0, 0.0, 3.0);
	_camera.front = Vec3d(0.0, 0.0, -1.0);
	_camera.up = Vec3d(0.0, 1.0, 0.0);
	_look_at = lookAt(_camera.pos, _camera.pos + _camera.front, _camera.up);

	return *this;
}

void Scene::addObject(const Object &object)
{
	_objs.push_back(object);
}

Tmatd Scene::lookAt(const Vec3d &eye, const Vec3d &at, const Vec3d &up)
{
	Vec3d n = (eye - at).normalize();
	Vec3d u = up.cross(n).normalize();
	Vec3d v = n.cross(u).normalize();

	return Tmatd(u(0), v(0), n(0), eye(0),
		u(1), v(1), n(1), eye(1),
		u(2), v(2), n(2), eye(2),
		0, 0, 0, 1);
}

void Scene::draw(ZBuffer &zbuffer, bool showFaceNormals, bool showVerNormals,
	bool givenFaceNormals, bool givenVertexNormals, bool showBoundingBox)
{
	Attr attr = { showFaceNormals ,showVerNormals, givenFaceNormals, givenVertexNormals, 
		showBoundingBox, Tmatd(), NEAR_PLANE , Shading::PHONG, _drawing_mode,
	_camera.pos};

	zbuffer.setLights(_lights);
	zbuffer.setAmbient(_ambient);

	for (auto &obj : _objs) {

		attr.T = _projection * _look_at * _view * obj.getModel();
		zbuffer.setAttributes(attr);
		zbuffer.draw(obj);	
	}

	if (!_is_initialized)
	{
		_is_initialized = true;
	}
}


Scene & Scene::setWireFrameColor(const COLORREF & color, int Id)
{
	if (Id == -1)
		for (auto& ob : _objs)
			for(auto& mesh : ob.getMeshes())
				mesh.setColor(color);
	else
		if (Id < _objs.size())
			for (auto& mesh : _objs[Id].getMeshes())
				mesh.setColor(color);

	return *this;
}


Scene & Scene::setFaceNormalsColor(const COLORREF & color, int Id)
{
	if (Id == -1)
		for (auto& ob : _objs)
			ob.setFNColor(color);
	else
		if (Id < _objs.size())
			_objs[Id].setFNColor(color);

	return *this;
}



Scene & Scene::setVerticesNormalsColor(const COLORREF & color, int Id)
{
	if (Id == -1)
		for (auto& ob : _objs)
			ob.setVNColor(color);
	else
		if (Id < _objs.size())
			_objs[Id].setVNColor(color);

	return *this;
}



Scene & Scene::setBoundingBoxColor(const COLORREF & color, int Id)
{
	if (Id == -1)
		for (auto& ob : _objs)
			ob.setBBColor(color);
	else
		if (Id < _objs.size())
			_objs[Id].setBBColor(color);

	return *this;
}

