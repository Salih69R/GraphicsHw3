#include "Poly.h"

Poly::Poly() :
	_vertices()
{

}

Poly &Poly::addVertex(const Vec4d &vertex)
{
	_vertices.emplace_back(vertex);

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
