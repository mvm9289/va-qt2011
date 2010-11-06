
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
		//int count = 0;

    for (ito = objects.begin(); ito != objects.end(); ito++)
		{			
				//cout<<"Pos del objeto "<<count<<": "<<(*ito).getPos()<<endl;
        (*ito).render(renderMode);
				//count++;
		}
}

void Scene::AddObject(Object &o)
{
    o.initGL();
    objects.push_back(o);
}

Point Scene::center()
{
    /*std::vector<Object>::iterator ito;
		Point cent;
		cent.x = cent.y = cent.z = 0;
	
		for (ito = objects.begin(); ito != objects.end(); ito++)
			cent += (*ito).boundingBox().center();

		cent = cent / objects.size();

    return cent;*/
		return objects[0].boundingBox().center();
}

float Scene::radius()
{
    Vector rad = objects[0].boundingBox().maxb - objects[0].boundingBox().center();
    
    return (rad.length());
}

void Scene::OpenModel(const char* filename)
{
		cout<<"Filename: "<<filename<<endl;
		Object o(filename);
    o.readObj(filename, matlib);
		o.setPos(objects[0].getPos());
    objects.push_back(o);
}

void Scene::setTexture(int textureID)
{
		cout<<"idSel = "<<idSel<<endl;
    objects[idSel].setTexture(textureID);
		idSel = -1;
}

void Scene::ChangeRenderMode(int mode)
{
    renderMode = mode;
}

vector<int> Scene::numTrianglesQuads_Model()
{
    return objects[0].numTrianglesQuads();
}


void Scene::novaRef(int id)
{

	cout<<"novaRef"<<endl;

  Object nwRef = objects[id];
  objects.push_back(nwRef);
}

void Scene::esborraNovaRef(int id)
{

	cout<<"esbNovaRef"<<endl;

	objects[id] = objects[objects.size()-1];
  objects.pop_back();
}

void Scene::cancelaMoviment()
{

	cout<<"cancela"<<endl;

	objects.pop_back();
}

void Scene::RenderNovaRef()
{
	
	cout<<"renderNovaRef"<<endl;

  objects[objects.size()-1].render(renderMode);
}

void Scene::IncPosNovaRef(float incX, float incY)
{
 
	cout<<"incpos"<<endl;

  Point p = objects[objects.size()-1].getPos();
  p.x += incX;
  p.z += incY;
  objects[objects.size()-1].setPos(p);
}

void Scene::setSelected(int id)
{
	idSel = id;
}

