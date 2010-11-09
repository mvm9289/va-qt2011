
#include "Scene.h"

MaterialLib Scene::matlib;

Scene::Scene() {}

void Scene::Init()
{
    renderMode = IMMEDIATE;
    selectedObjectID = NONE_OBJECT;
    
    Object o("Model");
    o.readObj("../models/teapot.obj", matlib);
    AddObject(o);
    updateBoundingBox();
}

void Scene::Render(bool projector)
{
    for (unsigned int i = 0; i < objects.size(); i++)
    {
        glPushName(i);
        objects[i].render(renderMode, projector);
        glPopName();
    }
    
    if (projector) boundingBox.renderRoom(EXTRA_SIZE_OF_ROOM);
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
    if(selectedObjectID != ALL_OBJECTS) objects[selectedObjectID].setTexture(textureID);
    else
        for (unsigned int i = 0; i < objects.size(); i++)
            objects[i].setTexture(textureID);
}

void Scene::repeatWrapS(int sWrap)
{
    if(selectedObjectID != ALL_OBJECTS) objects[selectedObjectID].repeatWrapS(sWrap);
    else
        for (unsigned int i = 0; i < objects.size(); i++)
            objects[i].repeatWrapS(sWrap);
}

void Scene::repeatWrapT(int tWrap)
{
    if(selectedObjectID != ALL_OBJECTS) objects[selectedObjectID].repeatWrapT(tWrap);
    else
        for (unsigned int i = 0; i < objects.size(); i++)
            objects[i].repeatWrapT(tWrap);
}

void Scene::redistributeSelectedObject(Point inc)
{
    Point p = objects[selectedObjectID].getPos();
    p += inc;
    objects[selectedObjectID].setPos(p);
}

void Scene::setSelected(int id, bool selected)
{
    if(id != ALL_OBJECTS) objects[id].setSelected(selected);
    else
        for (unsigned int i = 0; i < objects.size(); i++)
            objects[i].setSelected(selected);

    if (selected) selectedObjectID = id;
    else selectedObjectID = NONE_OBJECT;
}

void Scene::deleteSelectedModel()
{
    if(selectedObjectID != ALL_OBJECTS)
    {
        objects[selectedObjectID] = objects[objects.size() - 1];
        objects.pop_back();
    }
    else while(objects.size()) objects.pop_back();

    selectedObjectID = NONE_OBJECT;
}
