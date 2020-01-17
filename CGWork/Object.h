#include "Mesh.h"
#include "Draw.h"
#include "Drawable.h"

class Object : public Drawable
{
public:
	Object(CString str);
	~Object() = default;
	void addMesh(const Mesh& mesh);
	 std::vector<Mesh>& getMeshs()  { return _meshes; }
	Object &rotateX(const double angle_deg);
	Object &rotateY(const double angle_deg);
	Object &rotateZ(const double angle_deg);
	Object &translate(const Vec3d &translation);
	Object &scale(const Vec3d &scale);
	const Tmatd getModel() const;
	std::vector<std::pair<Vec4d, Vec4d>> getBoundingBoxLines();
	CString getName() { return _name; }
	const COLORREF getFNColor() const { return _fNormalColor; }
	Object& setFNColor(const COLORREF &color);
	const COLORREF getVNColor() const { return _vNormalColor; }
	Object& setVNColor(const COLORREF &color);
	const COLORREF getBBColor() const { return _BBColor; }
	Object& setBBColor(const COLORREF &color) {_BBColor = color;  return *this;}
	Vec3d getPos()const { return _pos; }
	virtual void draw(ZBuffer &zbuffer, const Attr &attr) override;


private: //methods
	Object &drawBoudingBox(ZBuffer &zbuffer, const Attr &attr);

private: //members
	std::vector<Mesh> _meshes;

	Tmatd _model;
	COLORREF _fNormalColor;
	COLORREF _vNormalColor;
	COLORREF _BBColor;
	CString _name;
	Vec3d _pos;
	BoundingBox _bounding_box;
};

