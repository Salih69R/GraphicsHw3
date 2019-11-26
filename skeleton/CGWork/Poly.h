#pragma once

#include <vector>

#include "Matrices.h"

class Poly
{
public:
	Poly();
	Poly &addVertex(const Vec4d &vertex);

	void SetFaceNormal(double x, double y, double z);
	void CalcSetFaceNormal(const Vec4d & vertex1, const Vec4d & vertex2, const Vec4d & vertex3);
	const Vec4d getAveragePosition() const;
	const Vec4d getFaceNormal() const;
	const std::vector<Vec4d> getVertices() const { return _vertices; }
	~Poly() = default;

private:
	std::vector<Vec4d> _vertices;
	std::vector<Vec4d> _vnormals;
	Vec4d _fnormal;
};

