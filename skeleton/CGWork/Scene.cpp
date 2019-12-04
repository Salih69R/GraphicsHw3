#include "Scene.h"

#define DEF_BOUNDING_BOX_SCREEN_RATION 3

Scene::Scene() :
	_meshes(),
	_view(),
	_projection(),
	_is_initialized(false),
	_background_color(RGB(0, 0, 0))
{
	_projection = TransformationMatrix<double>::ortho(-10.0, 10.0, -5.0, 5.0, -5.0, 5.0);

	Vec3d pos(0.0, 0.0, 3.0);
	Vec3d front(0.0, 0.0, -1.0);
	Vec3d up(0.0, 1.0, 0.0);
	lookAt(pos, pos + front, up);
}

void Scene::addMesh(const Mesh &model)
{
	_meshes.push_back(model);
}

Vec2u Scene::coordsToPixels(const double &x, const double &y, const uint &width, const uint &height)
{
	double width_d = static_cast<double>(width);
	double height_d = static_cast<double>(height);

	uint x_res = static_cast<uint>((width_d / 2.0) * (x + 1.0));
	uint y_res = static_cast<uint>((height_d / 2.0) * (1.0 - y));


	return Vec2u(x_res, y_res);
}

Scene &Scene::lookAt(const Vec3d &eye, const Vec3d &at, const Vec3d &up)
{
	Vec3d n = (eye - at).normalize();
	Vec3d u = up.cross(n).normalize();
	Vec3d v = n.cross(u).normalize();

	_view = Tmatd(u(0), v(0), n(0), -eye(0),
		u(1), v(1), n(1), -eye(1),
		u(2), v(2), n(2), -eye(2),
		0, 0, 0, 1);

	return *this;
}




void Scene::draw(int* bits, int width, int height, bool showFaceNormals, bool showVerNormals, bool givenFaceNormals, bool givenVertexNormals, bool showBoundingBox)
{










	for (auto &mesh : _meshes) {
		for (const auto &polygon : mesh.getPolygons())
		{

			//draw the polygons
			auto& vertexes = polygon.getVertices();
			Vec2u first_vertex_px;

			for (unsigned i = 0; i < vertexes.size() - 1; i++)
			{
				Vec4d p1 = _projection * _view * mesh.getModel() * vertexes[i];
				Vec4d p2 = _projection * _view * mesh.getModel() * vertexes[i + 1];

				p1 /= p1(3);
				p2 /= p2(3);

				auto px1 = coordsToPixels(p1(0), p1(1), width, height);
				auto px2 = coordsToPixels(p2(0), p2(1), width, height);

				if (i == 0)
				{
					first_vertex_px = px1;
				}

				MidPointDraw(px1(0), px1(1), px2(0), px2(1), bits,RGBToBGR( mesh.getColor()), width, height);

				if (i == vertexes.size() - 2)
				{
					MidPointDraw(first_vertex_px(0), first_vertex_px(1), px2(0), px2(1), bits, RGBToBGR(mesh.getColor()), width, height);
				}

			}
			//draw polgon normals
			if (showFaceNormals) {
				Vec4d p1 = _projection * _view *mesh.getModel() * polygon.getAveragePosition();
				Vec4d p2;
				if(givenFaceNormals && polygon.getGivenFaceNormal()(3)==1)//we flag _fGivenNormal(3)=0 (by default) if there isn't one
					p2 = _projection * _view *mesh.getModel() * polygon.getGivenFaceNormal();
				else
					p2 = _projection * _view *mesh.getModel() * polygon.getCalcFaceNormal();

				p1 /= p1(3);
				p2 /= p2(3);
				auto px1 = coordsToPixels(p1(0), p1(1), width, height);
				auto px2 = coordsToPixels(p2(0), p2(1), width, height);

				MidPointDraw(px1(0), px1(1), px2(0), px2(1), bits, RGBToBGR(mesh.getFNColor()), width, height);
			}
		}
			//draw vertices normals code
		if (showVerNormals) {
			std::vector<VertexAndNormal> vers = mesh.getVeritxes();
			for (unsigned i = 0; i < vers.size(); i++) {
				Vec4d p1 = _projection * _view *mesh.getModel() * vers[i]._vertex;
				Vec4d p2;
				
				if(givenVertexNormals && vers[i]._givenNormal(3)==1)//we flag _givenNormal(3)=0 (by default) if there isn't one
					p2 = _projection * _view *mesh.getModel() * (vers[i]._givenNormal + vers[i]._vertex);
				else 
					p2 = _projection * _view *mesh.getModel() * (vers[i]._calculatedNormal + vers[i]._vertex);

				p1 /= p1(3);
				p2 /= p2(3);
				auto px1 = coordsToPixels(p1(0), p1(1), width, height);
				auto px2 = coordsToPixels(p2(0), p2(1), width, height);

				MidPointDraw(px1(0), px1(1), px2(0), px2(1), bits, RGBToBGR(mesh.getVNColor()), width, height);
			}
		}
		
		
		//draw bounding box
		if (showBoundingBox) {
			auto bb_lines = mesh.getBoundingBoxLines();
			for (auto pair : bb_lines) {
				Vec4d p1 = _projection * _view *mesh.getModel() * pair.first;
				Vec4d p2;


					p2 = _projection * _view *mesh.getModel() * pair.second;


				p1 /= p1(3);
				p2 /= p2(3);
				auto px1 = coordsToPixels(p1(0), p1(1), width, height);
				auto px2 = coordsToPixels(p2(0), p2(1), width, height);

				MidPointDraw(px1(0), px1(1), px2(0), px2(1), bits, RGBToBGR(mesh.getBBColor()), width, height);
			}
			
	}

	}

	if (!_is_initialized)
	{
		_is_initialized = true;
	}
}




Scene & Scene::setWireFrameColor(const COLORREF & color, int mesh_Id)
{
	if (mesh_Id == -1)
		for (auto& mesh : _meshes)
			mesh.setColor(color);
	else
		if (mesh_Id < _meshes.size())
			_meshes[mesh_Id].setColor(color);

	return *this;
}


Scene & Scene::setFaceNormalsColor(const COLORREF & color, int mesh_Id)
{
	if (mesh_Id == -1)
		for (auto& mesh : _meshes)
			mesh.setFNColor(color);
	else
		if (mesh_Id < _meshes.size())
			_meshes[mesh_Id].setFNColor(color);

	return *this;
}



Scene & Scene::setVerticesNormalsColor(const COLORREF & color, int mesh_Id)
{
	if (mesh_Id == -1)
		for (auto& mesh : _meshes)
			mesh.setVNColor(color);
	else
		if (mesh_Id < _meshes.size())
			_meshes[mesh_Id].setVNColor(color);

	return *this;
}



Scene & Scene::setBoundingBoxColor(const COLORREF & color, int mesh_Id)
{
	if (mesh_Id == -1)
		for (auto& mesh : _meshes)
			mesh.setBBColor(color);
	else
		if (mesh_Id < _meshes.size())
			_meshes[mesh_Id].setBBColor(color);

	return *this;
}

