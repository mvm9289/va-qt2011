
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

    if (projective) initializeProj();

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

void Scene::initProjectiveMode(bool b)
{
   projective = b;
}

void Scene::initializeProj()
{
  GLfloat light0Pos[4] =
  {0.3, 0.3, 0.0, 1.0};
  GLfloat matAmb[4] =
  {0.01, 0.01, 0.01, 1.00};
  GLfloat matDiff[4] =
  {0.65, 0.65, 0.65, 1.00};
  GLfloat matSpec[4] =
  {0.30, 0.30, 0.30, 1.00};
  GLfloat matShine = 10.0;

  /* Setup Lighting */
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, matShine);

  glEnable(GL_COLOR_MATERIAL);

  glLightfv(GL_LIGHT1, GL_POSITION, light0Pos);
  glEnable(GL_LIGHT1);

  /*glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glEnable(GL_LIGHTING);*/

  /*glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGenf(GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGenf(GL_Q, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  GLfloat s[4] ={1.0f, 0.0f, 0.0f, 0.0f};
  GLfloat t[4] ={0.0f, 1.0f, 0.0f, 0.0f};
  GLfloat r[4] ={0.0f, 0.0f, 1.0f, 0.0f};
  GLfloat q[4] ={0.0f, 0.0f, 0.0f, 1.0f};
  glTexGenf(GL_S, GL_OBJECT_PLANE, s);
  glTexGenf(GL_T, GL_OBJECT_PLANE, t);
  glTexGenf(GL_R, GL_OBJECT_PLANE, r);
  glTexGenf(GL_Q, GL_OBJECT_PLANE, q);*/
}


void Scene::drawCube()
{

	glCullFace(GL_FRONT);
  glEnable(GL_CULL_FACE);

  glBegin(GL_QUADS);

  glNormal3f(-1.0, 0.0, 0.0);
  glColor3f(0.80, 0.50, 0.50);
  glVertex3f(-1.5, -1.5, -1.5);
  glVertex3f(-1.5, -1.5, 1.5);
  glVertex3f(-1.5, 1.5, 1.5);
  glVertex3f(-1.5, 1.5, -1.5);

  glNormal3f(1.0, 0.0, 0.0);
  glColor3f(0.50, 0.80, 0.50);
  glVertex3f(1.5, 1.5, 1.5);
  glVertex3f(1.5, -1.5, 1.5);
  glVertex3f(1.5, -1.5, -1.5);
  glVertex3f(1.5, 1.5, -1.5);

  glNormal3f(0.0, -1.0, 0.0);
  glColor3f(0.50, 0.50, 0.80);
  glVertex3f(-1.5, -1.5, -1.5);
  glVertex3f(1.5, -1.5, -1.5);
  glVertex3f(1.5, -1.5, 1.5);
  glVertex3f(-1.5, -1.5, 1.5);

  glNormal3f(0.0, 1.0, 0.0);
  glColor3f(0.50, 0.80, 0.80);
  glVertex3f(1.5, 1.5, 1.5);
  glVertex3f(1.5, 1.5, -1.5);
  glVertex3f(-1.5, 1.5, -1.5);
  glVertex3f(-1.5, 1.5, 1.5);

  glNormal3f(0.0, 0.0, -1.0);
  glColor3f(0.80, 0.50, 0.80);
  glVertex3f(-1.5, -1.5, -1.5);
  glVertex3f(-1.5, 1.5, -1.5);
  glVertex3f(1.5, 1.5, -1.5);
  glVertex3f(1.5, -1.5, -1.5);

  glNormal3f(0.0, 0.0, 1.0);
  glColor3f(1.00, 0.80, 0.50);
  glVertex3f(1.5, 1.5, 1.5);
  glVertex3f(-1.5, 1.5, 1.5);
  glVertex3f(-1.5, -1.5, 1.5);
  glVertex3f(1.5, -1.5, 1.5);
  glEnd();

	glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

}
