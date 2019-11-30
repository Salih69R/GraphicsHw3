#ifndef MODEL_H
#define MODEL_H


#include <afxwin.h>
#include <vector>

#include "Draw.h"
#include "Mesh.h"
#include "Matrices.h"

class Scene {
public:

	Scene();
	~Scene() = default;
	void addMesh(const Mesh& model);
	Vec2u coordsToPixels(const double &x, const double &y, const uint &width, const uint &height);
	Scene &lookAt(const Vec3d &eye, const Vec3d &at, const Vec3d &up);
	std::vector<Mesh> &getMeshes() { return _meshes; }
	Tmatd &getView() { return _view; }
	Scene &setInitialized(const bool &new_val) { _is_initialized = new_val; return *this; }
	bool isInitialized() const { return _is_initialized; }

	void draw(CDC * pDC, int width, int height, bool showFaceNormals, bool showVecNormals, bool givenFaceNormals, bool givenVertexNormals);//TODO: implement

private:
	std::vector<Mesh> _meshes;
	Tmatd _view;
	Tmatd _projection;
	bool _is_initialized;
};


#endif // !MODEL_H
