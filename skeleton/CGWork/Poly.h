#pragma once

#include <vector>

#include "Matrices.h"

class Poly
{
public:
	Poly();
	Poly &addVertex(const Vec4d &vertex);
	const std::vector<Vec4d> getVertices() const { return _vertices; }
	~Poly() = default;

private:
	std::vector<Vec4d> _vertices;
};

