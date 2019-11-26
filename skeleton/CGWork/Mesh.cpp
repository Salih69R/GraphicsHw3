#include "Mesh.h"



Mesh::Mesh() :
	_polygons(),
	_pos(0.0, 0.0, 0.0),
	_model(),
	_color(RGB(0, 0, 0))
{

}


Mesh &Mesh::addPolygon(const Poly &polygon)
{
	_polygons.emplace_back(polygon);

	return *this;
}

Mesh &Mesh::rotateX(const double angle_deg)
{
	_model.rotateX(angle_deg);

	return *this;
}

Mesh &Mesh::rotateY(const double angle_deg)
{
	_model.rotateY(angle_deg);

	return *this;
}

Mesh &Mesh::rotateZ(const double angle_deg)
{
	_model.rotateZ(angle_deg);

	return *this;
}

Mesh &Mesh::translate(const Vec3d &translation)
{
	_model.translate(translation);

	return *this;
}

Mesh &Mesh::scale(const Vec3d &scale)
{
	_model.scale(scale);

	return *this;
}