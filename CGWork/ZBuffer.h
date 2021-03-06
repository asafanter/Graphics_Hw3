#pragma once

#include <afxwin.h>

#include "Matrices.h"
#include "Light.h"
#include "Types.h"
#include "Object.h"
#include "PngWrapper.h"

class ZBuffer
{
public:
	enum BackgroundImageType
	{
		NONE,
		STRETCH,
		REPEAT
	};

	ZBuffer();
	ZBuffer(const uint &width, const uint &height, const Color &color = RGB(0, 0, 0));
	ZBuffer &resize(const uint &width, const uint &height);
	ZBuffer &set(const Pixel &p);
	void setBackgroundImage(const char *file_name, const BackgroundImageType &type);
	ZBuffer &setDefaultColor(const Color &color);
	Color getColor(const uint &x, const uint &y) const { return _bits[x + _width * y]; }
	uint getWidth() const { return _width; }
	uint getHeight() const { return _height; }
	bool isEmpty() const { return _bits == nullptr; }
	int *getBits() const { return _bits; }
	void draw(const Object &object);
	~ZBuffer();

private: //defs
	static constexpr uint MAX_LIGHTS = 7;


public: //methods
	Color RGBToBGR(COLORREF col);
	Color BGRToRGB(COLORREF col);
	Pixel nextPixel(const Pixel &p1, const Pixel &p2, const Pixel &p);
	Pixel nextPixelFill(const Pixel &start, const Pixel &target, const Pixel &p);
	int calcQuarter(const Pixel &p2, const Pixel &p, const double &m);
	void drawLine(const Pixel &p1, const Pixel &p2);
	Pixel toPixel(const Vertex &vertex, const Poly &polygon);
	Pixel interpolatePixel(const Pixel &p1, const Pixel &p2, const Pixel &p);
	void drawPolygonWireFrame(const Poly &polygon);
	void drawPolygonSolid(const Poly &polygon);
	void setLights(const std::vector<LightParams> &lights);
	void setAmbient(const LightParams &ambient) { _ambient = ambient; }
	void calcDrawOrder(Pixel &start, Pixel &via, Pixel &target, 
		const Poly &polygon);
	double calcWeight(const Pixel &p1, const Pixel &p2, const Pixel &p);
	void setAttributes(const Attr &attr) { _attr = attr; }
	Vec3 getLightColor(const LightParams &light) const;
	Vec3 colorToVec(const Color &color);
	Color vecToColor(const Vec3 &vec);
	Vec3 calcAmbient();
	Vec3 calcDiffuse(const Vec3 &pos, const Vec3 &normal);
	Vec3 calcSpecular(const Vec3 &pos, const Vec3 &normal);
	Vec3 calcSpotLight(const Vec3 &pos);
	Vec3 calcLightDir(const LightParams &light, const Vec3 &pos);
	Vec3 calcVertexNormal(const Vertex &vertex);
	Vec3 calcVertexPos(const Vertex &vertex);
	void calcCurrPolygonNormal(const Poly &polygon);
	void calcCurrPolygonPos(const Poly &polygon);
	void saveImageAsPng(const char* name);
	void saveImageAsPng(const char* name, const int *bits);
	void allocateLightsIntensities();
	void setEnableFog(const bool &val) { _is_foggy = val; }
	void setFogColor(const Color &color) { _fog_color = colorToVec(color); }
	double calcDepthInFog(const double &depth);
	Vec3 getValue(const int &row, const int &col);
	void setValue(const int &row, const int &col, const Color &color);
	void applyFilter(const std::vector<int> &kernel);
	Vec3 calcFilterColor(const int &row, const int &col, const std::vector<int> &kernel);
	void repeatImage(const int &w, const int &h);
	void stretchImage(const int &w, const int &h);
	void setSpotLightTheta(const double &theta) { _spot_light_theta = theta; }
	void setMinFog(const double &val) { _min_fog = val; }
	void setMaxFog(const double &val) { _max_fog = val; }
	void setPhongFactor(const int &val) { _phong_factor = val; }

private: //members
	int *_bits;
	std::vector<double> _z;
	std::vector<bool> _drawn;
	uint _width;
	uint _height;
	Color _color;
	std::vector<LightParams> _lights;
	Color _base_color;
	Attr _attr;
	LightParams _ambient;
	double _Ia;
	double _Id;
	double _Is;
	double _Ispot;
	Vec3 _curr_polygon_normal;
	Vec3 _curr_polygon_pos;
	PngWrapper _png;
	bool _is_foggy;
	Vec3 _fog_color;
	bool _has_background_image;
	double _spot_light_theta;
	double _min_fog;
	double _max_fog;
	int _phong_factor;
};

