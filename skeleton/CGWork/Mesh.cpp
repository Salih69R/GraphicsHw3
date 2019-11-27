#include "Mesh.h"
#include <algorithm>


Mesh::Mesh() :
	_polygons(),
	_pos(0.0, 0.0, 0.0),
	_model(),
	_color(RGB(0, 0, 0)),
	_fNormalColor(RGB(90, 55, 110)),//cool purple
	_vNormalColor(RGB(90, 215, 200))//cool cyan blue
{

}

Mesh &Mesh::addVertex(const VertexAndNormal &vertex)
{
	
	auto oldVertex = std::find(_vers.begin(), _vers.end(), vertex);
	if (oldVertex == _vers.end())
		_vers.emplace_back(vertex);

	Vec4d p = vertex._vertex;
	
	if (_vers.size() == 1)//first vertex
	{
		_maxX = _minX = p(0);
		_maxY = _minY = p(1);
		_maxZ = _minZ = p(2);
	}

	if (p(0) > _maxX)
		_maxX = p(0);
	if (p(1) > _maxY)
		_maxY = p(1);
	if (p(2) > _maxZ)
		_maxZ = p(2);

	if (p(0) < _minX)
		_minX = p(0);
	if (p(1) < _minY)
		_minY = p(1);
	if (p(2) < _minZ)
		_minZ = p(2);
	
	_pos = Vec3d((_maxX + _minX) / 2, (_maxY + _minY) / 2, (_maxZ + _minZ) / 2);

	return *this;
}

Mesh & Mesh::addVertexes(const std::vector<Vec4d>& vertexes, const Poly &polyHoldingThem)
{
	for (size_t i = 0; i < vertexes.size(); i++)
	{
		VertexAndNormal v;
		v._vertex = vertexes[i];
		v._polygonsTouching.push_back(polyHoldingThem);
		//this vertex might already be in the Mesh from another polygon or from adding it with a given normal
		
		auto oldVertex = std::find(_vers.begin(), _vers.end(), v);
		if (oldVertex != _vers.end()) {
			/* _vers contains v , so just update a new polygon for it*/
			oldVertex->_polygonsTouching.push_back(polyHoldingThem);
			
		}
		else {
			/* does not contain */
			addVertex(v);
		}
	}
	return *this;
}


Mesh & Mesh::addGivenNormal(Vec4d & vertex, Vec4d & normal)
{
	VertexAndNormal v;
	v._vertex = vertex;

	auto oldVertex = std::find(_vers.begin(), _vers.end(), v);
	if (oldVertex != _vers.end()) 
		oldVertex->_givenNormal = normal;
	else
	{
		v._givenNormal = normal;
		addVertex(v);
		
	}

	return *this;
}




Mesh & Mesh::calcVNormals()
{
	// TODO: insert return statement here
	for (size_t i = 0; i < _vers.size(); i++)
	{
		auto ver = _vers[i];
		Vec4d avg(0,0,0,0);
		
		for (auto pol : ver._polygonsTouching)
		{
			avg += pol._fnormal;
		}
		avg /= ver._polygonsTouching.size();
		avg(3) = 0;
		avg.normalize();
		avg(3) = 1;
		_vers[i]._calculatedNormal = avg;


	}

	return *this;
}

Mesh &Mesh::addPolygon(const Poly &polygon)
{
	_polygons.emplace_back(polygon);
	addVertexes(polygon._vertices, polygon);
	return *this;
}

const Tmatd Mesh::getModel() const
{
	Tmatd posmat;
	posmat.translate(_pos*-1);

	//m is max dimension the mesh takes
	double m = (_maxX - _pos(0));
	if ((_maxY - _pos(1)) > m)
		m = (_maxY - _pos(1));
	if ((_maxZ - _pos(2)) > m)
		m = (_maxZ - _pos(2));

	Tmatd norm;
	if(m != 0)
		norm.scale(Vec3d(1 / m,1/m,1/m));


	Tmatd mod = _model;

	return mod * norm *  posmat;
}


Mesh &Mesh::rotateX(const double angle_deg)
{
	_model.rotateX(angle_deg);

	return *this;
}

Mesh &Mesh::rotateY(const double angle_deg)
{
	_model.rotateY(angle_deg);

	return *this;
}

Mesh &Mesh::rotateZ(const double angle_deg)
{
	_model.rotateZ(angle_deg);

	return *this;
}

Mesh &Mesh::translate(const Vec3d &translation)
{
	_model.translate(translation);

	return *this;
}

Mesh &Mesh::scale(const Vec3d &scale)
{
	_model.scale(scale);

	return *this;
}