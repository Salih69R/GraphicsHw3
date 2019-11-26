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
	const Tmatd getModel() const { return _model; }
	const COLORREF getColor() const { return _color; }
	Mesh &setColor(const COLORREF &color) { _color = color; return *this; }

	~Mesh() = default;

private:
	std::vector<Poly> _polygons;
	Vec3d _pos;
	Tmatd _model;
	COLORREF _color;
};