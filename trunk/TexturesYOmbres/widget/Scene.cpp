
#include "Scene.h"

MaterialLib Scene::matlib;

Scene::Scene() {}

void Scene::Init()
{
    renderMode = 0;
    selectedObjectID = -1;
    
    Object o("Model");
    o.readObj("../models/queen.obj", matlib);
    AddObject(o);
    updateBoundingBox();
}

void Scene::Render()
{
    for (unsigned int i = 0; i < objects.size(); i++)
    {
        glPushName(i);
        objects[i].render(renderMode);
        glPopName();
    }
}

void Scene::AddObject(Object &o)
{
    o.initGL();
    objects.push_back(o);
    updateBoundingBox();
}

void Scene::updateBoundingBox()
{
    if (objects.size())
    {
        Box aux = objects[0].boundingBox();
        boundingBox = Box(aux.minb, aux.maxb);
        for (unsigned int i = 1; i < objects.size(); i++)
        {
            Box aux = objects[i].boundingBox();
            boundingBox.update(aux.minb);
            boundingBox.update(aux.maxb);
        }
    }
}

Point Scene::center()
{
    return boundingBox.center();
}

float Scene::radius()
{
    return ((boundingBox.maxb - boundingBox.minb).length()/2.0);
}

void Scene::OpenModel(const char* filename)
{
    Object o("Model");
    o.readObj(filename, matlib);
    AddObject(o);
    updateBoundingBox();
}

void Scene::ChangeRenderMode(int mode)
{
    renderMode = mode;
}

vector<int> Scene::numTrianglesQuads()
{
    vector<int> info(2, 0);
    for (unsigned int i = 0; i < objects.size(); i++)
    {
        vector<int> aux = objects[i].numTrianglesQuads();
        info[0] += aux[0];
        info[1] += aux[1];
    }
    
    return info;
}

void Scene::setTexture(int textureID)
{
    objects[selectedObjectID].setTexture(textureID);
}

void Scene::repeatWrapS(int sWrap)
{
    objects[selectedObjectID].repeatWrapS(sWrap);
}

void Scene::repeatWrapT(int tWrap)
{
    objects[selectedObjectID].repeatWrapT(tWrap);
}

void Scene::redistributeSelectedObject(float incX, float incY)
{
    Point p = objects[selectedObjectID].getPos();
    p.x += incX;
    p.z += incY;
    objects[selectedObjectID].setPos(p);
}

void Scene::setSelected(int id)
{
    selectedObjectID = id;
    objects[id].setSelected();
}

void Scene::setDeselected()
{
    objects[selectedObjectID].setDeselected();
    selectedObjectID = -1;
}

void Scene::deleteSelectedModel()
{
    objects[selectedObjectID] = objects[objects.size() - 1];
    objects.pop_back();
    selectedObjectID = -1;
}
