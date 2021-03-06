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
	_ambient(),
	_is_normals_fliped(false),
	_shading(Shading::PHONG),
	_back_face_culling(false),
	_is_foggy(false),
	_fog_color(RGB(127, 127, 127)),
	_filter(Filter::NONE),
	_is_recording_history(false),
	_need_save_history(false),
	_background_image_type(ZBuffer::BackgroundImageType::NONE),
	_background_image_file_name(),
	_spot_light_theta(7.0),
	_min_fog(1.0),
	_max_fog(1.0),
	_phong_factor(1)
{
	_projection = TransformationMatrix<double>::ortho(-10.0, 10.0, -5.0, 5.0, -5.0, 5.0);
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
	initHistory(zbuffer);

	Attr attr = { showFaceNormals ,showVerNormals, givenFaceNormals, givenVertexNormals, 
		showBoundingBox, Tmatd(), NEAR_PLANE , _shading, _drawing_mode,
	_camera.pos, _back_face_culling};

	zbuffer.setLights(_lights);
	zbuffer.setAmbient(_ambient);
	zbuffer.setEnableFog(_is_foggy);
	zbuffer.setFogColor(_fog_color);
	zbuffer.setSpotLightTheta(_spot_light_theta);
	zbuffer.setMinFog(_min_fog);
	zbuffer.setMaxFog(_max_fog);
	zbuffer.setPhongFactor(_phong_factor);

	for (auto &obj : _objs) {

		attr.T = _projection * _look_at * _view * obj.getModel();
		zbuffer.setAttributes(attr);
		zbuffer.draw(obj);	
	}

	if (_is_recording_history)
	{
		updateHistory(zbuffer);
	}

	if (_need_save_history)
	{
		zbuffer.saveImageAsPng("blur.png", _history);
		_need_save_history = false;
	}

	if (_filter != Filter::NONE)
	{
		auto kernel = createKernel();
		zbuffer.applyFilter(kernel);
	}

	if (_background_image_type != ZBuffer::BackgroundImageType::NONE)
	{
		zbuffer.setBackgroundImage(_background_image_file_name.c_str(), _background_image_type);
	}
	
	if (!_is_initialized)
	{
		_is_initialized = true;
	}
}

void Scene::saveHistory()
{
	_need_save_history = true;
}

void Scene::updateHistory(const ZBuffer &zbuffer)
{
	float factor = 0.9;

	int *bits = zbuffer.getBits();
	for (int i = 0; i < zbuffer.getWidth() * zbuffer.getHeight(); i++)
	{
		if (bits[i] != 0)
		{
			_history[i] = bits[i];
		}
		else
		{
			if (_history[i] != 0)
			{
				int r = GetBValue(_history[i]);
				int g = GetGValue(_history[i]);
				int b = GetRValue(_history[i]);

				Color color = RGB(b * factor, g * factor, r * factor);
				_history[i] = color;
			}
		}
	}
}

void Scene::initHistory(const ZBuffer &zbuffer)
{
	if (_history == nullptr)
	{
		int size = zbuffer.getWidth() * zbuffer.getHeight();
		_history = new int[size];
		for (int i = 0; i < size; i++)
		{
			_history[i] = 0;
		}
	}

}

Scene::~Scene()
{
	if (_history != nullptr)
	{
		delete[] _history;
	}
}
std::vector<int> Scene::createKernel()
{
	if (_filter == Filter::BOX3)
	{
		return createKernelBox3();
	}
	else if (_filter == Filter::BOX5)
	{
		return createKernelBox5();
	}
	else if (_filter == Filter::TRAINGLE3)
	{
		return createKernelTriangle3();
	}
	else if (_filter == Filter::TRAINGLE5)
	{
		return createKernelTriangle5();
	}
	else if (_filter == Filter::GAUSSIAN3)
	{
		return createKernelGaussian3();
	}
	else if (_filter == Filter::GAUSSIAN5)
	{
		return createKernelGaussian5();
	}
	else if (_filter == Filter::SINC3)
	{
		return createKernelSinc3();
	}
	else if (_filter == Filter::SINC5)
	{
		return createKernelSinc5();
	}
}

std::vector<int> Scene::createKernelBox3()
{
	std::vector<int> kernel;
	for (int i = 0; i < 9; i++)
	{
		kernel.push_back(1);
	}

	return kernel;
}

std::vector<int> Scene::createKernelBox5()
{
	std::vector<int> kernel;
	for (int i = 0; i < 25; i++)
	{
		kernel.push_back(1);
	}

	return kernel;
}

std::vector<int> Scene::createKernelTriangle3()
{
	std::vector<int> kernel;

	kernel.push_back(1);
	kernel.push_back(2);
	kernel.push_back(1);
	kernel.push_back(2);
	kernel.push_back(4);
	kernel.push_back(2);
	kernel.push_back(1);
	kernel.push_back(2);
	kernel.push_back(1);

	return kernel;
}

std::vector<int> Scene::createKernelTriangle5()
{
	std::vector<int> kernel;

	kernel.push_back(1);
	kernel.push_back(2);
	kernel.push_back(3);
	kernel.push_back(2);
	kernel.push_back(1);
	kernel.push_back(2);
	kernel.push_back(4);
	kernel.push_back(6);
	kernel.push_back(4);
	kernel.push_back(2);
	kernel.push_back(3);
	kernel.push_back(6);
	kernel.push_back(9);
	kernel.push_back(6);
	kernel.push_back(3);
	kernel.push_back(2);
	kernel.push_back(4);
	kernel.push_back(6);
	kernel.push_back(4);
	kernel.push_back(2);
	kernel.push_back(1);
	kernel.push_back(2);
	kernel.push_back(3);
	kernel.push_back(2);
	kernel.push_back(1);

	return kernel;
}

std::vector<int> Scene::createKernelGaussian3()
{
	std::vector<int> kernel;

	kernel.push_back(1);
	kernel.push_back(2);
	kernel.push_back(1);
	kernel.push_back(2);
	kernel.push_back(5);
	kernel.push_back(2);
	kernel.push_back(1);
	kernel.push_back(2);
	kernel.push_back(1);

	return kernel;
}

std::vector<int> Scene::createKernelGaussian5()
{
	std::vector<int> kernel;

	kernel.push_back(1);
	kernel.push_back(1);
	kernel.push_back(1);
	kernel.push_back(1);
	kernel.push_back(1);
	kernel.push_back(1);
	kernel.push_back(2);
	kernel.push_back(4);
	kernel.push_back(2);
	kernel.push_back(1);
	kernel.push_back(1);
	kernel.push_back(4);
	kernel.push_back(10);
	kernel.push_back(4);
	kernel.push_back(1);
	kernel.push_back(1);
	kernel.push_back(2);
	kernel.push_back(4);
	kernel.push_back(2);
	kernel.push_back(1);
	kernel.push_back(1);
	kernel.push_back(1);
	kernel.push_back(1);
	kernel.push_back(1);
	kernel.push_back(1);

	return kernel;
}

std::vector<int> Scene::createKernelSinc3()
{
	std::vector<int> kernel;

	kernel.push_back(2);
	kernel.push_back(3);
	kernel.push_back(2);
	kernel.push_back(3);
	kernel.push_back(4);
	kernel.push_back(3);
	kernel.push_back(2);
	kernel.push_back(3);
	kernel.push_back(2);

	return kernel;
}

std::vector<int> Scene::createKernelSinc5()
{
	std::vector<int> kernel;

	kernel.push_back(-2);
	kernel.push_back(-1);
	kernel.push_back(0);
	kernel.push_back(-1);
	kernel.push_back(-2);
	kernel.push_back(-1);
	kernel.push_back(4);
	kernel.push_back(6);
	kernel.push_back(4);
	kernel.push_back(-1);
	kernel.push_back(0);
	kernel.push_back(6);
	kernel.push_back(9);
	kernel.push_back(6);
	kernel.push_back(0);
	kernel.push_back(-1);
	kernel.push_back(4);
	kernel.push_back(6);
	kernel.push_back(4);
	kernel.push_back(-1);
	kernel.push_back(-2);
	kernel.push_back(-1);
	kernel.push_back(0);
	kernel.push_back(-1);
	kernel.push_back(-2);

	return kernel;
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

void Scene::setBackgroundImage(const std::string &file_name)
{
	_background_image_file_name = file_name;
}
