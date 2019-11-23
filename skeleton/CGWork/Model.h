#ifndef MODEL_H
#define MODEL_H

#include "Matrices.h"
#include <vector>
#include <Windows.h>

using std::vector;



class Model
{
	vector<Vec3d> vertexes; //in the objects world
	COLORREF color;
	double pos; //in the screen world
	Tmatd mTransform;//mat4 mTransform;

public:
	Model(COLORREF col);
	virtual ~Model();
	void draw();//might need to be pure virual in case we'd want to draw differently for every Model type

	void addVertex(Vec3d vertex);
	void translate(const Vec3d &translation);
	void scale(const float &scalar);
	void rotate(const Mat3d &rotation);
	const vector<Vec3d> getVertexes();
	COLORREF getColor();
};



class Camera 
{
	Tmatd cTranform;
	Tmatd projection;

public:
	void setTransformation(const Tmatd& T);//replaces
	void setProjection(const Tmatd& T);//replaces
	Tmatd LookAt(Vec3d& eye, Vec3d& at, Vec3d& up);
	
	/*
	void Ortho(...);
	void Persspective(...);
	...
	*/
};




class Renderer {

	/*

	float *m_outBuffer; // 3*width*height
	float *m_zbuffer; // width*height
	int m_width, m_height;
	void CreateBuffers(int width, int height);
public:
//construction
	void Init();
	void DrawTriangles(vector<vec3>* vertices)
	void SetCameraTransform(mat4& cTransform);
	void SetProjection(mat4& projection);
	void SetObjectMatrix(mat4& oTransform, mat3& nTransform);
	void SwapBuffers(); }
	*/
	
};


class Scene {
	
vector<Model*> models;
vector<Camera*> cameras;
Renderer *m_renderer;
public:

	Scene(Renderer *renderer);
	void AddModel(Model* model);
	void AddCamera(Camera* camera);
	Model* GetModel(int model_id);
	
	//…

	void draw();//TODO: implement
};


#endif // !MODEL_H
