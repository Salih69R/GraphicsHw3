#include "Poly.h"

Poly::Poly() :
	_vertices() , _vnormals()
{

}

Poly &Poly::addVertex(const Vec4d &vertex)
{
	_vertices.emplace_back(vertex);
	
	if (_vertices.size() == 1)//first vertex
	{
		_maxX = _minX = vertex(0);
		_maxY = _minY = vertex(1);
		_maxZ = _minZ = vertex(2);
	}

	if (vertex(0) > _maxX)
		_maxX = vertex(0);
	if (vertex(1) > _maxY)
		_maxY = vertex(1);
	if (vertex(2) > _maxZ)
		_maxZ = vertex(2);

	if (vertex(0) < _minX)
		_minX = vertex(0);
	if (vertex(1) < _minY)
		_minY = vertex(1);
	if (vertex(2) < _minZ)
		_minZ = vertex(2);

	return *this;
}



void Poly::SetFaceNormal(double x, double y, double z)
{
	_fnormal = Vec4d(x, y, z, 1.0);
}


void Poly::CalcSetFaceNormal(const Vec4d & vertex1, const Vec4d & vertex2, const Vec4d & vertex3)
{
	Vec4d u = vertex3 - vertex1;
	Vec4d v = vertex2 - vertex1;

	_fnormal = u.cross(v);
}

const Vec4d Poly::getAveragePosition() const
{
	Vec4d avg = _vertices[0] + _vertices[1] + _vertices[2];
	avg /= 3;

	return avg;
}



const Vec4d Poly::getFaceNormal() const
{
	Vec4d avg = getAveragePosition();

	//Tmatd pos;
	//pos.translate(avg(0),avg(1),avg(2));

	return avg + _fnormal;
}
