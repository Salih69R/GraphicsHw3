#include "Mesh.h"


class Object
{
public:
	Object(CString str);
	~Object() = default;

	void addMesh(const Mesh& mesh);
	 std::vector<Mesh>& getMeshs()  { return _meshs; }
	


	Object &rotateX(const double angle_deg);
	Object &rotateY(const double angle_deg);
	Object &rotateZ(const double angle_deg);
	Object &translate(const Vec3d &translation);
	Object &scale(const Vec3d &scale);
	const Tmatd getModel() const;

	//returns std::vector with size 12, has 12 lines (pairs of vertexes) of the bounding box
	std::vector<std::pair<Vec4d, Vec4d>> getBoundingBoxLines();


	CString getName() { return _name; }
	const COLORREF getFNColor() const { return _fNormalColor; }
	Object& setFNColor(const COLORREF &color) {
		_fNormalColor = color;  return *this;
	}

	const COLORREF getVNColor() const { return _vNormalColor; }
	Object& setVNColor(const COLORREF &color) {
		_vNormalColor = color;  return *this;
	}
	
	const COLORREF getBBColor() const { return _BBColor; }
	Object& setBBColor(const COLORREF &color) {
		_BBColor = color;  return *this;
	}
	Vec3d getPos()const { return _pos; }



private:
	std::vector<Mesh> _meshs;

	Tmatd _model;//in Object
	COLORREF _fNormalColor;//in Object
	COLORREF _vNormalColor;//in Object
	COLORREF _BBColor;//in Object
	CString _name;//in Object


	Vec3d _pos;
	//in Object
	//used for bounding box and fixing the starting pos
	double _maxX;
	double _maxY;
	double _maxZ;
	double _minX;
	double _minY;
	double _minZ;
};

