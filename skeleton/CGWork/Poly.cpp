#include "Poly.h"

Poly::Poly() :
	_vertices() 
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



void Poly::SetGivenFaceNormal(double x, double y, double z)
{
	_fGivenNormal = Vec4d(x, y, z, 0);
	_fGivenNormal.normalize();
	_fGivenNormal(3) = 1.0;//the 1 on the end also flags that it has been given, otherwise (0) falgs that it hasn't
}


void Poly::CalcSetFaceNormal()
{
	Vec4d u = _vertices[1] - _vertices[0];
	Vec4d v = _vertices[2] - _vertices[1];

	_fnormal = u.cross(v);
	_fnormal.normalize();
}

const Vec4d Poly::getAveragePosition() const
{
	Vec4d avg = _vertices[0];
	
	for (size_t i = 1; i < _vertices.size(); i++)
	{
		avg += _vertices[i];
	}
	
	avg /= _vertices.size();

	return avg;
}



const Vec4d Poly::getCalcFaceNormal() const
{
	Vec4d avg = getAveragePosition();
	Tmatd pos;
	pos.translate(avg(0), avg(1), avg(2));


	return pos * _fnormal;
}

const Vec4d Poly::getGivenFaceNormal() const
{
	Vec4d avg = getAveragePosition();
	Tmatd pos;
	pos.translate(avg(0), avg(1), avg(2));

	return pos * _fGivenNormal;
}
