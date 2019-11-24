#include "Model.h"



Model::Model(COLORREF col) // default is black
{
	color = col;
}


Model::~Model()
{
}


//TODO: implement
void Model::draw()
{

}

void Model::addVertex(Vec3d vertex)
{
	vertexes.push_back(vertex);
}

void Model::translate(const Vec3d & translation)
{
	mTransform.translate(translation);
}

void Model::scale(const float & scalar)
{
	mTransform *= scalar;
}

void Model::rotate(const Mat3d & rotation)
{
	mTransform.setRotation(rotation);
}

const vector<Vec3d> Model::getVertexes()
{
	return vertexes;
}

COLORREF Model::getColor()
{
	return color;
}




Tmatd Camera::LookAt(Vec3d & eye, Vec3d & at, Vec3d & up)
{
	Vec3d n = (eye - at).normalize();
	Vec3d u = up.cross(n).normalize();
	Vec3d v = n.cross(u).normalize();

	Tmatd c = Tmatd( u(0), v(0), n(0), 0.0, u(1), v(1), n(1), 0.0 , u(2), v(2), n(2), 0.0 , 0, 0, 0, 1);//c = (u, v, n, t) in colums

	Vec3d i = eye;
	i *= -1;	

	return c.translate(i);
}




void Camera::setTransformation(const Tmatd & T)
{
	cTranform = T;
}

void Camera::setProjection(const Tmatd & T)
{
	projection = T;
}

Scene::Scene(Renderer * renderer)
{
	m_renderer = renderer;
}

void Scene::AddModel(Model * model)
{
	models.push_back(model);
}

void Scene::AddCamera(Camera * camera)
{
	cameras.push_back(camera);
}

Model * Scene::GetModel(int model_id)
{
	return models[model_id];
}

void Scene::draw()
{
	//sends the renderer all "the geometry", so we can just send it the whole model (all models)
}
