#pragma once

#include <afxwin.h>

#include "Poly.h"


class VertexAndNormal {
public:
	
	Vec4d _vertex, _givenNormal, _calculatedNormal;
	std::vector<Poly> _polygonsTouching;


	/*the fact the givenNormal(3) is 0 flags that it still has not gotten any given normal, 
	and so if we come to draw the given normal and the 4th value is still 0, we print the calculated one instead*/
	VertexAndNormal():_givenNormal(0,0,0,0), _calculatedNormal(0,0,0,0), _polygonsTouching(){}
	
	friend bool operator==(const VertexAndNormal& first, const VertexAndNormal& other) {
		return (first._vertex(0) == other._vertex(0)) && (first._vertex(1) == other._vertex(1)) && (first._vertex(2) == other._vertex(2) && (first._vertex(3) == other._vertex(3)));
	}
};




class Mesh
{
public:
	Mesh(COLORREF col, char* str);
	Mesh &addVertex(const VertexAndNormal &vertex);
	Mesh &addVertexes(const std::vector<Vec4d>& vertexes, const Poly &polyHoldingThem);
	const std::vector<VertexAndNormal>& getVeritxes() const { return _vers; }

	//if the vertex of this normal is not in the mesh yet, it adds it, it's ok because it will be added anyway with the polygon
	//(won't make another one then, it will just add the polygon to the vertex)
	Mesh &addGivenNormal(Vec4d& vertex, Vec4d& normal);
	Mesh &calcVNormals();
	Mesh &addPolygon(const Poly &polygon);
	const std::vector<Poly>& getPolygons() const { return _polygons; }

	Mesh &rotateX(const double angle_deg);
	Mesh &rotateY(const double angle_deg);
	Mesh &rotateZ(const double angle_deg);
	Mesh &translate(const Vec3d &translation);
	Mesh &scale(const Vec3d &scale);
	const Tmatd getModel() const;



	const COLORREF getColor() const { return _color; }
	Mesh& setColor(const COLORREF &color) { 
		_color = color;  return *this;
	}


	Vec4d getPos() { return Vec4d(_pos(0), _pos(1), _pos(2), 1.0); }

	
	//returns std::vector with size 12, has 12 lines (pairs of vertexes) of the bounding box
	std::vector<std::pair<Vec4d, Vec4d>> getBoundingBoxLines();
	CString getName() { return _name; }


	~Mesh() = default;
	friend class Object;
private:
	std::vector<Poly> _polygons;
	std::vector<VertexAndNormal> _vers;	
	Vec3d _pos;
	Tmatd _model;//in Object
	COLORREF _color;;//in Object
	CString _name;//in Object

	//in Object
	//used for bounding box and fixing the starting pos
	double _maxX;
	double _maxY;
	double _maxZ;
	double _minX;
	double _minY;
	double _minZ;
};