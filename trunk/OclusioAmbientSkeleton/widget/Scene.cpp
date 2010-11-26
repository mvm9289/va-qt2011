#include "Scene.h"

MaterialLib Scene::matlib;

Scene::Scene()
{
}

void Scene::Init()
{  
	Object* o = new Object("queen");
	o->readObj("../models/monkey.obj", matlib);
	AddObject(o);
}

// Mètode que pinta l'escena: el terra, els objectes i el vehicle
void Scene::Render()
{
	// Pintar tots els objectes de l'escena 
	std::vector<Object*>::iterator ito;
	for (ito=objects.begin(); ito!=objects.end(); ito++)
		(*ito)->render(); 
}

void Scene::AddObject(Object* o)
{
	o->initGL();
	objects.push_back(o);
}

Point Scene::Center()
{
	// Se tienen que tener en cuenta todos los objetos!!
	return (objects[0]->boundingBox().Center());
}

float Scene::RadiEscena()
{
	Vector rad = objects[0]->boundingBox().maxb - objects[0]->boundingBox().Center();
	return (rad.length());
}
