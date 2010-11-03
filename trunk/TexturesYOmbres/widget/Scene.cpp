
#include "Scene.h"

MaterialLib Scene::matlib;

Scene::Scene() {}

void Scene::Init()
{
    renderMode = 0;
    
    Object o("Model");
    o.readObj("../models/cotxe.obj", matlib);
    AddObject(o);
}

void Scene::Render()
{
    std::vector<Object>::iterator ito;
    for (ito = objects.begin(); ito != objects.end(); ito++)
        (*ito).render(renderMode);
}

void Scene::AddObject(Object &o)
{
    o.initGL();
    objects.push_back(o);
}

Point Scene::center()
{
    return objects[0].boundingBox().center();
}

float Scene::radius()
{
    Vector rad = objects[0].boundingBox().maxb - objects[0].boundingBox().center();
    
    return (rad.length());
}

void Scene::OpenModel(const char* filename)
{
    objects[0].readObj(filename, matlib);
    objects[0].initGL();
}

void Scene::setTexture(void* textureData, int width, int height)
{
    objects[0].setTexture(textureData, width, height);
}

void Scene::ChangeRenderMode(int mode)
{
    renderMode = mode;
}

vector<int> Scene::numTrianglesQuads_Model()
{
    return objects[0].numTrianglesQuads();
}
