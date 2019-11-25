#ifndef MODEL_H
#define MODEL_H

#include "Matrices.h"
#include <vector>
#include <afxwin.h>
#include "Draw.h"

using std::vector;



class Model
{
	vector<Vec4d> vertexes; //in the objects world
	COLORREF color;
	double pos; //in the screen world
	Tmatd mTransform;//mat4 mTransform;

public:
	Model(COLORREF col);
	virtual ~Model();
	void draw();//might need to be pure virual in case we'd want to draw differently for every Model type

	void addVertex(double x, double y, double z);


	void translate(const Vec3d &translation);

	//TODO: should we allow scale in various scales for every axis?
	void scale(const float &scalar);

	//rotate
	void rotateX(const double angle_deg);
	void rotateY(const double angle_deg);
	void rotateZ(const double angle_deg);

	const vector<Vec4d> getModeledVertexes();
	COLORREF getColor();
};



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
	
vector<Model*> models;
vector<Camera*> cameras;

public:

	Scene();
	~Scene();
	void AddModel(Model* model);
	void AddCamera(Camera* camera);
	Model& GetModel(int model_id);
	Camera& GetCamera(int camera_id);
	

	void draw(CDC * pDC, int width, int height);//TODO: implement
};


#endif // !MODEL_H
