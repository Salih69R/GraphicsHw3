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

	void draw(CDC * pDC, int width, int height, bool showFaceNormals, bool showVecNormals, bool givenFaceNormals, bool givenVertexNormals);//TODO: implement

private:
	std::vector<Mesh> _meshes;
	Tmatd _view;
	Tmatd _projection;
};


#endif // !MODEL_H
