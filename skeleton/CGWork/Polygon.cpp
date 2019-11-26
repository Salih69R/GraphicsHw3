#include "Polygon.h"



Polygon &Polygon::addVertex(const Vec4d &vertex)
{
	_vertices.emplace_back(vertex);
}