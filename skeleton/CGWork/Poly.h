#pragma once

#include <vector>

#include "Matrices.h"
extern class Mesh;
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


	friend class Mesh;//because why not
private:
	std::vector<Vec4d> _vertices;
	Vec4d _fnormal;

	//used for bounding box
	double _maxX;
	double _maxY;
	double _maxZ;
	double _minX;
	double _minY;
	double _minZ;
};

