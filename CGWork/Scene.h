#ifndef MODEL_H
#define MODEL_H


#include <afxwin.h>
#include <vector>

#include "Object.h"
#include "Matrices.h"
#include "ZBuffer.h"


class Scene {
public:

	static constexpr double FOV = 45.0;
	static constexpr double NEAR_PLANE = 0.1;
	static constexpr double FAR_PLANE = 20.0;

	Scene();
	~Scene() = default;
	void addObject(const Object& object);
	Tmatd lookAt(const Vec3d &eye, const Vec3d &at, const Vec3d &up);
	std::vector<Object> &getObjects() { return _objs; }
	Tmatd &getView() { return _view; }
	Scene &setInitialized(const bool &new_val) { _is_initialized = new_val; return *this; }
	bool isInitialized() const { return _is_initialized; }
	Scene &setProjection(const Tmatd &projection) { _projection = projection; return *this; }
	Scene &reset();
	void flipNormals();
	void setBuffer(const int &w, const int &h);

	COLORREF getBackgroundColor() const { return _background_color; }
	Scene& setBackgroundColor(const COLORREF &color) { _background_color = color; return *this; }

	Scene& setWireFrameColor(const COLORREF &color, int Id);
	Scene& setFaceNormalsColor(const COLORREF &color, int Id);
	Scene& setVerticesNormalsColor(const COLORREF &color, int Id);
	Scene& setBoundingBoxColor(const COLORREF &color, int Id);
	bool isNormalsFliped() const { return _is_normals_fliped; }
	void setDrawingMode(const DrawingMode &drawing_mode) { _drawing_mode = drawing_mode; }
	void addLights(const std::vector<LightParams> &lights) { _lights = lights; }
	void addAmbient(const LightParams &ambient) { _ambient = ambient; }
	void draw(ZBuffer &zbuffer, bool showFaceNormals, bool showVecNormals, bool givenFaceNormals, bool givenVertexNormals, bool showBoundingBox) ;

private:

	struct Camera
	{
		Vec3d pos;
		Vec3d front;
		Vec3d up;
	};

	std::vector<Object> _objs;
	Tmatd _view;
	Tmatd _projection;
	bool _is_initialized;
	COLORREF _background_color;
	Camera _camera;
	Tmatd _look_at;
	DrawingMode _drawing_mode;
	std::vector<LightParams> _lights;
	LightParams _ambient;
	bool _is_normals_fliped;
	ZBuffer _zbuffer;
};


#endif // !MODEL_H
