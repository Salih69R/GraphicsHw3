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
	mTransform.setTranslation(translation);
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




Tmatd Camera::LookAt(Vec4d & eye, Vec4d & at, Vec4d & up)
{
	Vec4d n = eye;
	n -= at;
	n.normalize();

	Vec4d u = up;
	u.cross(n).normalize();
	
	Vec4d v = n;
	v.cross(u).normalize();

	Vec4d t = Vec4d(0.0, 0.0, 0.0, 1.0);


	Tmatd c = Tmatd( u(0), v(0), n(0), t(0), u(1), v(1), n(1), t(1), u(2), v(2), n(2), t(2), u(3), v(3), n(3), t(3));//c = (u, v, n, t) in colums

	Vec4d i = eye;
	i *= -1;
	

	return c.Translation(Vec3d(i(0),i(1),i(2)));
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
