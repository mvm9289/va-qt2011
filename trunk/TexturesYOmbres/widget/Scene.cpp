
#include "Scene.h"

MaterialLib Scene::matlib;

Scene::Scene() {}

void Scene::Init()
{
    renderMode = 0;
    projective = false;    

    Object o("Model");
    o.readObj("../models/cotxe.obj", matlib);
    AddObject(o);
}

void Scene::Render()
{
    if(projective)
    {
     initializeProj();
     drawCube();

     glDisable(GL_LIGHT1);
    }

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


void Scene::initProjectiveMode(bool b)
{
    projective = b;
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

}
