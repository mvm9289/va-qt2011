
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
    return objects[0].boundingBox().center();///////////////////////////////////////////////////////////////////PODER AÑADIR MAS
}

float Scene::radius()
{
    Vector rad = objects[0].boundingBox().maxb - objects[0].boundingBox().center();///////////////////////////////////////////////////////////////////PODER AÑADIR MAS
    
    return (rad.length());
}

void Scene::OpenModel(const char* filename)
{
    objects[0].readObj(filename, matlib);///////////////////////////////////////////////////////////////////PODER AÑADIR MAS
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

void Scene::setTexture(int textureID)
{
    objects[0].setTexture(textureID);
}

void Scene::repeatWrapS(int sWrap)
{
    objects[0].repeatWrapS(sWrap);
}

void Scene::repeatWrapT(int tWrap)
{
    objects[0].repeatWrapT(tWrap);
}

void Scene::novaRef(int id)
{
    Object nwRef = objects[id];
    objects.push_back(nwRef);
}

void Scene::esborraNovaRef(int id)
{
    objects[id] = objects[objects.size()-1];
    objects.pop_back();
}

void Scene::cancelaMoviment()
{
    objects.pop_back();
}

void Scene::RenderNovaRef()
{
    objects[objects.size()-1].render(renderMode);
}

void Scene::IncPosNovaRef(float incX, float incY)
{
    Point p = objects[objects.size()-1].getPos();
    p.x += incX;
    p.z += incY;
    objects[objects.size()-1].setPos(p);
}

void Scene::setSelected(int id)
{
    idSel = id;
}
