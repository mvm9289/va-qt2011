
#include "Scene.h"

MaterialLib Scene::matlib;
    
Scene::Scene() {}

void Scene::Init()
{
    renderMode = OCCLUSION;
    selectedObjectID = NONE_OBJECT;
    shadows = false;
    
    Object o("Model");
    o.readObj("../models/monkey.obj", matlib);
    AddObject(o);
}

void Scene::Render(bool projector)
{
    if (shadows) renderShadows();
    else
    {
        renderObjects(projector);
        if (projector) boundingBox.renderRoom(EXTRA_SIZE_OF_ROOM);
    }
}

void Scene::renderObjects(bool projector)
{
    for (unsigned int i = 0; i < objects.size(); i++)
    {
        glPushName(i);
        objects[i].render(renderMode, projector);
        glPopName();
    }
}

void Scene::AddObject(Object &o)
{
    o.initGL();
    objects.push_back(o);

    //updateBoundingBox();

    o.updateAmbientOcclusion(20, objects);
    objects.pop_back();
    objects.push_back(o);
    
    //o.updateObscurances(5, (float)(boundingBox.diagonal()/1.5), true, objects);
    //cout<<"dmax que he puesto: "<<(float)(boundingBox.diagonal()/1.5)<<endl;
    //objects.pop_back();
    //objects.push_back(o);

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

void Scene::setShadows()
{
    if (shadows)
    {
        glDisable(GL_LIGHT1);
        glEnable(GL_LIGHT0);
        shadows = false;
    }
    else
    {
        glDisable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        light = Light(GL_LIGHT1, center(), radius());
        light.sendPositionToGL();
        shadows = true;
    }
}

void Scene::renderShadows()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    for (int i = 0; i < 6; i++)
    {
        glEnable(GL_LIGHTING);
        glClear(GL_STENCIL_BUFFER_BIT);
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_ALWAYS, 1, 1);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        vector<float> pC = boundingBox.renderRoom(EXTRA_SIZE_OF_ROOM, i);

        glDisable(GL_DEPTH_TEST);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glStencilFunc(GL_EQUAL, 1, 1);
        glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
        glPushMatrix();
        Point P = light.getPos();
        GLfloat projectionMatrix[16] = {-pC[3] - pC[1]*P.y - pC[2]*P.z, pC[0]*P.y, pC[0]*P.z, pC[0],
                                                        pC[1]*P.x, -pC[3] - pC[0]*P.x - pC[2]*P.z, pC[1]*P.z, pC[1],
                                                        pC[2]*P.x, pC[2]*P.y, -pC[3] - pC[0]*P.x - pC[1]*P.y, pC[2],
                                                        pC[3]*P.x, pC[3]*P.y, pC[3]*P.z, -pC[0]*P.x - pC[1]*P.y - pC[2]*P.z};
        glMultMatrixf(projectionMatrix);
        renderObjects();
        glPopMatrix();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0, 0, 0, 0.8);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDisable(GL_LIGHTING);
        glStencilFunc(GL_EQUAL, 0, 1);
        boundingBox.renderRoom(EXTRA_SIZE_OF_ROOM, i);
        glDisable(GL_BLEND);

        glEnable(GL_LIGHTING);
        glDepthFunc(GL_LESS);
        glDisable(GL_STENCIL_TEST);
        renderObjects();
    }
    glPushMatrix();
    glTranslatef(light.getPos().x, light.getPos().y, light.getPos().z);
    glDisable(GL_LIGHTING);
    glColor3f(0.2, 0.2, 0.8);
    gluSphere(gluNewQuadric(), (5*radius())/100, 10, 10);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}


void Scene::setLightRadius(float rad)
{
    light.setRadius(rad);
    light.sendPositionToGL();
}
void Scene::setLightLongitude(float lon)
{
    light.setLongitude(lon);
    light.sendPositionToGL();
}
void Scene::setLightLatitude(float lat)
{
    light.setLatitude(lat);
    light.sendPositionToGL();
}
