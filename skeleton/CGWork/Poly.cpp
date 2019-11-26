#include "Poly.h"

Poly::Poly() :
	_vertices()
{

}

Poly &Poly::addVertex(const Vec4d &vertex)
{
	_vertices.emplace_back(vertex);
}