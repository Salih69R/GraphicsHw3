#pragma once

#include <afxwin.h>

#include "Poly.h"

class Mesh
{
public:
	Mesh();
	Mesh &addPolygon(const Poly &polygon);
	Mesh &rotateX(const double angle_deg);
	Mesh &rotateY(const double angle_deg);
	Mesh &rotateZ(const double angle_deg);
	Mesh &translate(const Vec3d &translation);
	Mesh &scale(const Vec3d &scale);
	const std::vector<Poly> getPolygons() const { return _polygons; }
	const Tmatd getModel() const;
	const COLORREF getColor() const { return _color; }
	const COLORREF getFNColor() const { return _fNormalColor; }
	const COLORREF getVNColor() const { return _vNormalColor; }
	Mesh &setColor(const COLORREF &color) { _color = color; return *this; }
	Vec4d getPos() { return Vec4d(_pos(0), _pos(1), _pos(2), 1.0); }

	~Mesh() = default;

private:
	std::vector<Poly> _polygons;
	Vec3d _pos;
	Tmatd _model;
	COLORREF _color;
	COLORREF _fNormalColor;
	COLORREF _vNormalColor;
	
	//used for bounding box and fixing the starting pos
	double _maxX;
	double _maxY;
	double _maxZ;
	double _minX;
	double _minY;
	double _minZ;
};