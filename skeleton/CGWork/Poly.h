#pragma once

#include <vector>

#include "Matrices.h"
class Mesh;
class Poly
{
public:
	Poly();
	Poly &addVertex(const Vec4d &vertex);

	void SetGivenFaceNormal(double x, double y, double z);
	void CalcSetFaceNormal();
	const Vec4d getAveragePosition() const;
	const Vec4d getCalcFaceNormal() const;
	const Vec4d getGivenFaceNormal() const;
	const std::vector<Vec4d> getVertices() const { return _vertices; }
	~Poly() = default;


	friend class Mesh;//because why not
private:
	std::vector<Vec4d> _vertices;
	Vec4d _fnormal;
	Vec4d _fGivenNormal;

	//used for bounding box
	double _maxX;
	double _maxY;
	double _maxZ;
	double _minX;
	double _minY;
	double _minZ;
};

