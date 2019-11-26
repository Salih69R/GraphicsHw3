#pragma once

#include <vector>

#include "Matrices.h"

class Polygon
{
public:
	Polygon() = default;
	Polygon &addVertex(const Vec4d &vertex);
	const std::vector<Vec4d> getVertices() const { return _vertices; }
	~Polygon() = default;

private:
	std::vector<Vec4d> _vertices;
};

