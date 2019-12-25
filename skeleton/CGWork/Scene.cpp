#include "Scene.h"

#define DEF_BOUNDING_BOX_SCREEN_RATION 3

Scene::Scene() :
	_objs(),
	_view(),
	_projection(),
	_is_initialized(false),
	_background_color(RGB(0, 0, 0)),
	_camera({}),
	_look_at()
{
	_projection = TransformationMatrix<double>::ortho(-10.0, 10.0, -5.0, 5.0, -5.0, 5.0);
	_camera.pos = Vec3d(0.0, 0.0, 3.0);
	_camera.front = Vec3d(0.0, 0.0, -1.0);
	_camera.up = Vec3d(0.0, 1.0, 0.0);

	_look_at = lookAt(_camera.pos, _camera.pos + _camera.front, _camera.up);
}

Scene &Scene::reset()
{
	_objs.clear();
	_view = Tmatd();
	_projection = TransformationMatrix<double>::ortho(-10.0, 10.0, -5.0, 5.0, -5.0, 5.0);
	_is_initialized = false;
	_background_color = RGB(0, 0, 0);
	_camera.pos = Vec3d(0.0, 0.0, 3.0);
	_camera.front = Vec3d(0.0, 0.0, -1.0);
	_camera.up = Vec3d(0.0, 1.0, 0.0);
	_look_at = lookAt(_camera.pos, _camera.pos + _camera.front, _camera.up);

	return *this;
}

void Scene::addObject(const Object &object)
{
	_objs.push_back(object);
}


Tmatd Scene::lookAt(const Vec3d &eye, const Vec3d &at, const Vec3d &up)
{
	Vec3d n = (eye - at).normalize();
	Vec3d u = up.cross(n).normalize();
	Vec3d v = n.cross(u).normalize();

	return Tmatd(u(0), v(0), n(0), -eye(0),
		u(1), v(1), n(1), -eye(1),
		u(2), v(2), n(2), -eye(2),
		0, 0, 0, 1);
}




void Scene::drawPoly(const Poly& polygon,Tmatd& transformation, int* bits,double* zdepth, int width, int height, bool showFaceNormals, bool givenFaceNormals, COLORREF color, COLORREF faceNormalsColor)
{


	for (unsigned i = 0; i < height*width; ++i) {

		zdepth[i] = FAR_PLANE;
	}

	//draw the polygons
	auto& vertexes = polygon.getVertices();
	Vec2u first_vertex_px;
	double first_vertex_zdepth;


	Poly p = polygon;



	ScaneConvert(p, transformation, bits,zdepth, width, height, color, NEAR_PLANE);



	for (unsigned i = 0; i < vertexes.size() - 1; i++)
	{
		Vec4d p1 = transformation * vertexes[i];
		Vec4d p2 = transformation * vertexes[i + 1];

		if (p1(3) < NEAR_PLANE) p1(3) = NEAR_PLANE;
		if (p2(3) < NEAR_PLANE) p2(3) = NEAR_PLANE;

		p1 /= p1(3);
		p2 /= p2(3);

		auto px1 = coordsToPixels(p1(0), p1(1), width, height);
		auto px2 = coordsToPixels(p2(0), p2(1), width, height);



		if (i == 0)
		{
			first_vertex_px = px1;
			first_vertex_zdepth = p1(2);
		}

		MidPointDraw(px1(0), px1(1),p1(2), px2(0), px2(1),p2(2), bits, zdepth, RGBToBGR(color), width, height);

		if (i == vertexes.size() - 2)
		{
			MidPointDraw(first_vertex_px(0), first_vertex_px(1), first_vertex_zdepth, px2(0), px2(1),p2(2) , bits, zdepth, RGBToBGR(color), width, height);
		}

	}
	//draw polgon face normals
	if (showFaceNormals) {
		Vec4d p1 = transformation * polygon.getAveragePosition();
		Vec4d p2;
		if (givenFaceNormals && polygon.getGivenFaceNormal()(3) == 1)//we flag _fGivenNormal(3)=0 (by default) if there isn't one
			p2 = transformation * polygon.getGivenFaceNormal();
		else
			p2 = transformation * polygon.getCalcFaceNormal();

		p1 /= p1(3);
		p2 /= p2(3);
		auto px1 = coordsToPixels(p1(0), p1(1), width, height);
		auto px2 = coordsToPixels(p2(0), p2(1), width, height);

		MidPointDraw(px1(0), px1(1), p1(2), px2(0), px2(1), p2(2), bits, zdepth, RGBToBGR(faceNormalsColor), width, height);
	}
}

void Scene::drawMeshVerticeNormals(Mesh& mesh,Tmatd& transformation, int* bits, double* zdepth, int width, int height ,bool givenVertexNormals, COLORREF verticesNormalColor)
{
	std::vector<VertexAndNormal> vers = mesh.getVeritxes();
	for (unsigned i = 0; i < vers.size(); i++) {
		Vec4d p1 = transformation * vers[i]._vertex;
		Vec4d p2;
		Tmatd pos;
		pos.translate(vers[i]._vertex(0), vers[i]._vertex(1), vers[i]._vertex(2));

		if (givenVertexNormals && vers[i]._givenNormal(3) == 1)//we flag _givenNormal(3)=0 (by default) if there isn't one
			p2 = transformation * pos *(vers[i]._givenNormal);
		else
			p2 = transformation * pos * (vers[i]._calculatedNormal);

		p1 /= p1(3);
		p2 /= p2(3);
		auto px1 = coordsToPixels(p1(0), p1(1), width, height);
		auto px2 = coordsToPixels(p2(0), p2(1), width, height);

		MidPointDraw(px1(0), px1(1),p1(2), px2(0), px2(1),p1(2), bits,zdepth, RGBToBGR(verticesNormalColor), width, height);
	}
}



void Scene::drawObjectBoundingBox(Object& obj,Tmatd& transformation, int* bits, double* zdepth, int width, int height)
{
	auto bb_lines = obj.getBoundingBoxLines();
	for (auto pair : bb_lines) {
		Vec4d p1 = transformation * pair.first;
		Vec4d p2;


		p2 = transformation * pair.second;


		p1 /= p1(3);
		p2 /= p2(3);
		auto px1 = coordsToPixels(p1(0), p1(1), width, height);
		auto px2 = coordsToPixels(p2(0), p2(1), width, height);

		MidPointDraw(px1(0), px1(1),p1(2), px2(0), px2(1), p2(2), bits, zdepth, RGBToBGR(obj.getBBColor()), width, height);
	}
}


void Scene::draw(int* bits, double* zdepth, int width, int height, bool showFaceNormals, bool showVerNormals, bool givenFaceNormals, bool givenVertexNormals, bool showBoundingBox)
{
	
	for (auto &obj : _objs) {

		auto transformation = _projection * _look_at * _view * obj.getModel();

		for (auto &mesh : obj.getMeshs())
		{
			for (const auto &polygon : mesh.getPolygons())
			{
				//draw the polygons
				drawPoly(polygon, transformation, bits, zdepth, width, height, showFaceNormals, givenFaceNormals, mesh.getColor(), obj.getFNColor());
			}
				//draw mesh vertices normals code
			if (showVerNormals) {
				drawMeshVerticeNormals(mesh, transformation, bits, zdepth, width, height, givenVertexNormals, obj.getVNColor());
			}
		}

			//draw object bounding box
			if (showBoundingBox) {
				drawObjectBoundingBox(obj, transformation, bits, zdepth, width, height);
			}

	}

	if (!_is_initialized)
	{
		_is_initialized = true;
	}
}



//works on the meshes inside the objects
Scene & Scene::setWireFrameColor(const COLORREF & color, int Id)
{
	if (Id == -1)//set for all
		for (auto& ob : _objs)
			for(auto& mesh : ob.getMeshs())
				mesh.setColor(color);
	else
		if (Id < _objs.size())
			for (auto& mesh : _objs[Id].getMeshs())
				mesh.setColor(color);

	return *this;
}


Scene & Scene::setFaceNormalsColor(const COLORREF & color, int Id)
{
	if (Id == -1)
		for (auto& ob : _objs)
			ob.setFNColor(color);
	else
		if (Id < _objs.size())
			_objs[Id].setFNColor(color);

	return *this;
}



Scene & Scene::setVerticesNormalsColor(const COLORREF & color, int Id)
{
	if (Id == -1)
		for (auto& ob : _objs)
			ob.setVNColor(color);
	else
		if (Id < _objs.size())
			_objs[Id].setVNColor(color);

	return *this;
}



Scene & Scene::setBoundingBoxColor(const COLORREF & color, int Id)
{
	if (Id == -1)
		for (auto& ob : _objs)
			ob.setBBColor(color);
	else
		if (Id < _objs.size())
			_objs[Id].setBBColor(color);

	return *this;
}

