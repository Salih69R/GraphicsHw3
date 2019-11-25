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

void Model::addVertex(double x, double y, double z)
{
	//turn it to Vec4d because we need them to be ready for drawing
	Vec4d ver(x, y, z, 1);

	vertexes.push_back(ver);
}

void Model::translate(const Vec3d & translation)
{
	mTransform.translate(translation);
}


//TODO: should we allow scale in various scales for every axis?
void Model::scale(const float & scalar)
{
	mTransform *= scalar;
}

void Model::rotateX(const double angle_deg)
{
	mTransform.rotateX(angle_deg);
}

void Model::rotateY(const double angle_deg)
{
	mTransform.rotateY(angle_deg);
}

void Model::rotateZ(const double angle_deg)
{
	mTransform.rotateZ(angle_deg);
}



const vector<Vec4d> Model::getModeledVertexes()
{
	
	vector<Vec4d> ret;

	for (auto &ver : vertexes) {
		ret.push_back(mTransform * ver);
	}
	
	return ret;
}

COLORREF Model::getColor()
{
	return color;
}
















void Camera::LookAt(Vec3d & eye, Vec3d & at, Vec3d & up)
{
	Vec3d n = (eye - at).normalize();
	Vec3d u = up.cross(n).normalize();
	Vec3d v = n.cross(u).normalize();

	Tmatd c = Tmatd( u(0), v(0), n(0), 0.0, u(1), v(1), n(1), 0.0 , u(2), v(2), n(2), 0.0 , 0, 0, 0, 1);//c = (u, v, n, t) in colums

	Vec3d i = eye;
	i *= -1;	

	cTranform = c.translate(i);
}

void Camera::Ortho(double left, double right, double top, double bottom, double, double)
{

	//TODO: set projection = ....
}

Vec4d Camera::toProjectionView(Vec4d vertex)
{
	return projection * (cTranform * vertex);
}




Camera::Camera()
{
	
	cTranform.translate(0, 0, -5);//default is I

	Ortho(-5, 5, 5, -5, -5, 5);
}

void Camera::setTransformation(const Tmatd & T)
{
	cTranform = T;
}

void Camera::setProjection(const Tmatd & T)
{
	projection = T;
}



















Scene::Scene()
{
	Camera* cam = new Camera();
	AddCamera(cam);
}

Scene::~Scene()
{
	for (size_t i = 0; i < models.size(); i++)
	{
		delete models[i];
	}
	for (size_t i = 0; i < cameras.size(); i++)
	{
		delete cameras[i];
	}
}

void Scene::AddModel(Model * model)
{
	models.push_back(model);
}

void Scene::AddCamera(Camera * camera)
{
	cameras.push_back(camera);
}


Model& Scene::GetModel(int model_id)
{
	return *(models[model_id]);
}

Camera& Scene::GetCamera(int camera_id)
{
	return *(cameras[camera_id]);
}

void Scene::draw(CDC * pDC, int width, int height)
{

	Camera* cam = cameras[0];//we have only one camera

	for (Model* model : models) {

		auto& vertexes = model->getModeledVertexes();

		for (unsigned i = 0; i < vertexes.size() - 1; i++) {

			Vec4d point1 = cam->toProjectionView(vertexes[i]);
			Vec4d point2 = cam->toProjectionView(vertexes[i + 1]);

			//Salih: Testing stuff: x=(x+1)*w , y= (1-y)*h/2
			MidPointDraw((point1(0) + 1)* width / 2, (point1(1) + 1)* height / 2, (point2(0) + 1)* width / 2, (point2(1) + 1)* height / 2, pDC, model->getColor());
		}

	}
}

