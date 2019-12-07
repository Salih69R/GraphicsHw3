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

Vec2u Scene::coordsToPixels(const double &x, const double &y, const uint &width, const uint &height)
{
	double width_d = static_cast<double>(width);
	double height_d = static_cast<double>(height);

	uint x_res = static_cast<uint>((width_d / 2.0) * (x + 1.0));
	uint y_res = static_cast<uint>((height_d / 2.0) * (1.0 - y));


	return Vec2u(x_res, y_res);
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




void Scene::draw(int* bits, int width, int height, bool showFaceNormals, bool showVerNormals, bool givenFaceNormals, bool givenVertexNormals, bool showBoundingBox)
{
	for (auto &obj : _objs) {

		auto transformation = _projection * _look_at * _view * obj.getModel();

		for (auto &mesh : obj.getMeshs())
		{
			for (const auto &polygon : mesh.getPolygons())
			{

				//draw the polygons
				auto& vertexes = polygon.getVertices();
				Vec2u first_vertex_px;

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
					}

					MidPointDraw(px1(0), px1(1), px2(0), px2(1), bits,RGBToBGR(mesh.getColor()), width, height);

					if (i == vertexes.size() - 2)
					{
						MidPointDraw(first_vertex_px(0), first_vertex_px(1), px2(0), px2(1), bits, RGBToBGR(mesh.getColor()), width, height);
					}

				}
				//draw polgon face normals
				if (showFaceNormals) {
					Vec4d p1 = transformation * polygon.getAveragePosition();
					Vec4d p2;
					if(givenFaceNormals && polygon.getGivenFaceNormal()(3)==1)//we flag _fGivenNormal(3)=0 (by default) if there isn't one
						p2 = transformation * polygon.getGivenFaceNormal();
					else
						p2 = transformation * polygon.getCalcFaceNormal();

					p1 /= p1(3);
					p2 /= p2(3);
					auto px1 = coordsToPixels(p1(0), p1(1), width, height);
					auto px2 = coordsToPixels(p2(0), p2(1), width, height);

					MidPointDraw(px1(0), px1(1), px2(0), px2(1), bits, RGBToBGR(obj.getFNColor()), width, height);
				}
			}
				//draw mesh vertices normals code
			if (showVerNormals) {
				std::vector<VertexAndNormal> vers = mesh.getVeritxes();
				for (unsigned i = 0; i < vers.size(); i++) {
					Vec4d p1 = transformation * vers[i]._vertex;
					Vec4d p2;
				
					if(givenVertexNormals && vers[i]._givenNormal(3)==1)//we flag _givenNormal(3)=0 (by default) if there isn't one
						p2 = transformation * (vers[i]._givenNormal + vers[i]._vertex);
					else 
						p2 = transformation * (vers[i]._calculatedNormal + vers[i]._vertex);

					p1 /= p1(3);
					p2 /= p2(3);
					auto px1 = coordsToPixels(p1(0), p1(1), width, height);
					auto px2 = coordsToPixels(p2(0), p2(1), width, height);

					MidPointDraw(px1(0), px1(1), px2(0), px2(1), bits, RGBToBGR(obj.getVNColor()), width, height);
				}
			}
		


		}

			//draw object bounding box
			if (showBoundingBox) {
				auto bb_lines = obj.getBoundingBoxLines();
				for (auto pair : bb_lines) {
					Vec4d p1 = transformation * pair.first;
					Vec4d p2;


						p2 = transformation * pair.second;


					p1 /= p1(3);
					p2 /= p2(3);
					auto px1 = coordsToPixels(p1(0), p1(1), width, height);
					auto px2 = coordsToPixels(p2(0), p2(1), width, height);

					MidPointDraw(px1(0), px1(1), px2(0), px2(1), bits, RGBToBGR(obj.getBBColor()), width, height);
				}
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

