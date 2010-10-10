#include "Scene.h"

MaterialLib Scene::matlib;

Scene::Scene()
{
}

void Scene::Init()
{  
	renderMode = 0;

	Object o("model");
	o.readObj("../models/mini_obj.obj", matlib);
	AddObject(o);
}

// Mètode que pinta l'escena: el terra, els objectes i el vehicle
void Scene::Render()
{
	// Pintar tots els objectes de l'escena 
	std::vector<Object>::iterator ito;
	for (ito=objects.begin(); ito!=objects.end(); ito++)
		(*ito).render(renderMode); 
}

void Scene::AddObject(Object &o)
{
	o.initGL();
	objects.push_back(o);
}

Point Scene::Center()
{
	// Se tienen que tener en cuenta todos los objetos!!
	return (objects[0].boundingBox().Center());
}

float Scene::RadiEscena()
{
	Vector rad = objects[0].boundingBox().maxb - objects[0].boundingBox().Center();
	return (rad.length());
}

void Scene::OpenModel(const char* filename)
{
	objects[0].readObj(filename, matlib);
	objects[0].initGL();
}

void Scene::ChangeRenderMode(int mode)
{
	renderMode = mode;
}

vector<int> Scene::numTrianglesQuads_Model()
{
	return objects[0].numTrianglesQuads();
}


