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

	enum Filter
	{
		NONE,
		BOX3,
		BOX5,
		TRAINGLE3,
		TRAINGLE5,
		GAUSSIAN3,
		GAUSSIAN5,
		SINC3,
		SINC5
	};

	Scene();
	~Scene();
	void addObject(const Object& object);
	Tmatd lookAt(const Vec3d &eye, const Vec3d &at, const Vec3d &up);
	std::vector<Object> &getObjects() { return _objs; }
	Tmatd &getView() { return _view; }
	Scene &setInitialized(const bool &new_val) { _is_initialized = new_val; return *this; }
	bool isInitialized() const { return _is_initialized; }
	Scene &setProjection(const Tmatd &projection) { _projection = projection; return *this; }
	Scene &reset();
	void setShading(const Shading &new_shading) { _shading = new_shading; }
	void flipNormals();
	void setEnableFog(const bool &val) { _is_foggy = val; }
	void setFogColor(const Color &color) { _fog_color = color; }
	bool isFoggy() const { return _is_foggy; }
	void setFilter(const Filter &filter_type) { _filter = filter_type; }
	std::vector<int> createKernel();
	std::vector<int> createKernelBox3();
	std::vector<int> createKernelBox5();
	std::vector<int> createKernelTriangle3();
	std::vector<int> createKernelTriangle5();
	std::vector<int> createKernelGaussian3();
	std::vector<int> createKernelGaussian5();
	std::vector<int> createKernelSinc3();
	std::vector<int> createKernelSinc5();

	COLORREF getBackgroundColor() const { return _background_color; }
	bool getBackFaceCulling() const { return _back_face_culling; }
	DrawingMode getDrawingMode() const { return _drawing_mode; }
	Scene& setBackgroundColor(const COLORREF &color) { _background_color = color; return *this; }

	Scene& setWireFrameColor(const COLORREF &color, int Id);
	Scene& setFaceNormalsColor(const COLORREF &color, int Id);
	Scene& setVerticesNormalsColor(const COLORREF &color, int Id);
	Scene& setBoundingBoxColor(const COLORREF &color, int Id);
	void setBackFaceCulling(const bool &val) { _back_face_culling = val; }
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
	Shading _shading;
	bool _back_face_culling;
	bool _is_foggy;
	Color _fog_color;
	Filter _filter;
	int *_history;
};


#endif // !MODEL_H
