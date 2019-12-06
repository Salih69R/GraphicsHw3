#ifndef MODEL_H
#define MODEL_H


#include <afxwin.h>
#include <vector>

#include "Draw.h"
#include "Object.h"
#include "Matrices.h"



class Scene {
public:

	Scene();
	~Scene() = default;
	void addMesh(const Object& model);//mesh is the theoritical name, but here it object, mesh is a part of the object (cow object has meshes: face, tail...)
	Vec2u coordsToPixels(const double &x, const double &y, const uint &width, const uint &height);
	Scene &lookAt(const Vec3d &eye, const Vec3d &at, const Vec3d &up);
	std::vector<Object> &getMeshes() { return _objs; }//mesh is the theoritical name, but here it object, mesh is a part of the object (cow object has meshes: face, tail...)
	Tmatd &getView() { return _view; }
	Scene &setInitialized(const bool &new_val) { _is_initialized = new_val; return *this; }
	bool isInitialized() const { return _is_initialized; }
	Scene &setProjection(const Tmatd &projection) { _projection = projection; return *this; }

	COLORREF getBackgroundColor() const { return _background_color; }
	Scene& setBackgroundColor(const COLORREF &color) { _background_color = color; return *this; }

	Scene& setWireFrameColor(const COLORREF &color, int Id);
	Scene& setFaceNormalsColor(const COLORREF &color, int Id);//to all the  if Id = -1
	Scene& setVerticesNormalsColor(const COLORREF &color, int Id);//to all the  if Id = -1
	Scene& setBoundingBoxColor(const COLORREF &color, int Id);//to all the  if Id = -1



	void draw(int * bits, int width, int height, bool showFaceNormals, bool showVecNormals, bool givenFaceNormals, bool givenVertexNormals, bool showBoundingBox) ;
private:
	std::vector<Object> _objs;
	//std::vector<Mesh> _meshes;
	Tmatd _view;
	Tmatd _projection;
	bool _is_initialized;
	COLORREF _background_color;
};


#endif // !MODEL_H
