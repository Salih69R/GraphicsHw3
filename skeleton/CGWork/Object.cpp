#include "Object.h"



Object::Object( CString str) : 
	_model(),
	_name(str)
{

}

void Object::addMesh(const Mesh &model)
{
	_meshs.push_back(model);

	//update bounding box and position

	if (_meshs.size() == 1)//first one to add
	{
		_maxX = _meshs[0]._minX;
		_maxY = _meshs[0]._minY;
		_maxZ = _meshs[0]._minZ;

		_minX = _meshs[0]._minX;
		_minY = _meshs[0]._minY;
		_minZ = _meshs[0]._minZ;

		//set colors
		_fNormalColor = _vNormalColor = _BBColor = _meshs[0].getColor();
	}

	for (int i = 0; i < _meshs.size(); ++i) {
		auto& mesh = _meshs[i];

		
		if (mesh._maxX > _maxX)
			_maxX = mesh._maxX;
		if (mesh._maxY > _maxY)
			_maxY = mesh._maxY;
		if (mesh._maxZ > _maxZ)
			_maxZ = mesh._maxZ;

		if (mesh._minX < _minX)
			_minX = mesh._minX;
		if (mesh._minY < _minY)
			_minY = mesh._minY;
		if (mesh._minZ < _minZ)
			_minZ = mesh._minZ;
	}
	_pos = Vec3d((_maxX + _minX) / 2, (_maxY + _minY) / 2, (_maxZ + _minZ) / 2);
}

Object & Object::rotateX(const double angle_deg)
{
	_model.rotateX(angle_deg);

	return *this;
}

Object & Object::rotateY(const double angle_deg)
{
	_model.rotateY(angle_deg);

	return *this;
}

Object & Object::rotateZ(const double angle_deg)
{
	_model.rotateZ(angle_deg);

	return *this;
}

Object & Object::translate(const Vec3d & translation)
{
	_model.translate(translation);

	return *this;
}

Object & Object::scale(const Vec3d & scale)
{
	_model.scale(scale);

	return *this;
}

const Tmatd Object::getModel() const
{
	return _model;
}

std::vector<std::pair<Vec4d, Vec4d>> Object::getBoundingBoxLines()
{
	Vec4d p1(_maxX, _maxY, _maxZ, 1.0);
	Vec4d p2(_maxX, _maxY, _minZ, 1.0);
	Vec4d p3(_minX, _maxY, _minZ, 1.0);
	Vec4d p4(_minX, _maxY, _maxZ, 1.0);
	Vec4d p5(_maxX, _minY, _maxZ, 1.0);
	Vec4d p6(_minX, _minY, _maxZ, 1.0);
	Vec4d p7(_minX, _minY, _minZ, 1.0);
	Vec4d p8(_maxX, _minY, _minZ, 1.0);


	std::pair<Vec4d, Vec4d> l1(p1, p2), l2(p2, p3), l3(p3, p4), l4(p4, p1), l5(p5, p6), l6(p6, p7), l7(p7, p8), l8(p8, p5), l9(p1, p5), l10(p2, p8), l11(p3, p7), l12(p4, p6);

	std::vector<std::pair<Vec4d, Vec4d>> container;

	container.push_back(l1);
	container.push_back(l2);
	container.push_back(l3);
	container.push_back(l4);
	container.push_back(l5);
	container.push_back(l6);
	container.push_back(l7);
	container.push_back(l8);
	container.push_back(l9);
	container.push_back(l10);
	container.push_back(l11);
	container.push_back(l12);

	return container;
}

