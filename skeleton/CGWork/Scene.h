#ifndef MODEL_H
#define MODEL_H

#include "Matrices.h"
#include <vector>
#include <afxwin.h>
#include "Draw.h"
#include "Mesh.h"

//as it is, doesn't support "expanding" the already set cTransformaion and projection
class Camera 
{
	Tmatd cTranform;
	Tmatd projection;

public:


	Camera();//default Camera pos is 0,0,-5, def projection is orthographic with cube (10X10X10) from -5 to 5 in every axis
	void setTransformation(const Tmatd& T);//replaces
	void setProjection(const Tmatd& T);//replaces
	void LookAt(Vec3d& eye, Vec3d& at, Vec3d& up);//replaces the mTransform
	void Ortho(double left, double right, double top, double bottom, double near, double far);//replaces the projection 
	Vec4d toProjectionView(Vec4d vertex);
	/*TODO:
	void Persspective(...); //replaces the projection 
	*/
};



class Scene {
public:

	Scene();
	~Scene() = default;
	void addMesh(const Mesh& model);
	Vec2u coordsToPixels(const double &x, const double &y, const uint &width, const uint &height);
	std::vector<Mesh> &getMeshes() { return _meshes; }
	

	void draw(CDC * pDC, int width, int height, bool showFaceNormals, bool showVecNormals);//TODO: implement

private:
	std::vector<Mesh> _meshes;
};


#endif // !MODEL_H
