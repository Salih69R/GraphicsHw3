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

	if (_polygons.size() == 1)//first vertex
	{
		_maxX = polygon._maxX;
		_minX = polygon._minX;
		_maxY = polygon._maxY;
		_minY = polygon._minY;
		_maxZ = polygon._maxZ;
		_minZ = polygon._minZ;
	}

	if (polygon._maxX > _maxX)
		_maxX = polygon._maxX;
	if (polygon._maxY > _maxY)
		_maxY = polygon._maxY;
	if (polygon._maxZ > _maxZ)
		_maxZ = polygon._maxZ;

	if (polygon._minX < _minX)
		_minX = polygon._minX;
	if (polygon._minY < _minY)
		_minY = polygon._minY;
	if (polygon._minZ < _minZ)
		_minZ = polygon._minZ;

	_pos = Vec3d((_maxX + _minX) / 2, (_maxY + _minY) / 2, (_maxZ + _minZ) / 2);

	return *this;
}

const Tmatd Mesh::getModel() const
{
	Tmatd posmat;
	posmat.translate(_pos*-1);

	//m is max dimension the mesh takes
	int m = (_maxX - _pos(0));
	if ((_maxY - _pos(1)) > m)
		m = (_maxY - _pos(1));
	if ((_maxZ - _pos(2)) > m)
		m = (_maxZ - _pos(2));

	Tmatd norm;
	if(m != 0)
		norm.scale(Vec3d(1 / m,1/m,1/m));


	Tmatd mod = _model;

	return mod * norm *  posmat;
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