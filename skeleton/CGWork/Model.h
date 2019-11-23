#include<vector>


#ifndef MODEL_H
#define MODEL_H



using std::vector;

class Model
{
protected:
	Model();
	virtual ~Model();
	void virtual draw() = 0;
	//mat4 mTransform;
	//Geometry T
};



class Camera {

	//mat4 cTranform;
	//mat4 projection;

public:
	/*
	void setTransformation(const mat4& T);
	void setProjection(const mat4& T);
	mat4 LookAt(vec4& eye, vec4& at, vec4& up);
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
	/*
vector<Model*> models;
vector<Camera*> cameras;
Renderer *m_renderer;
public:

	Scene(Renderer *renderer);
	void AddModel(Model* model);
	void AddCamera(Model* model);
	Model* GetModel(int model_id);
	…

	void draw();
	*/
};


#endif // !MODEL_H
