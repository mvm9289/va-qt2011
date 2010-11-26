#ifndef _Scene_H_
#define _Scene_H_
#include <QtOpenGL/qgl.h>

#include "Object.h"

using namespace std;

class Scene
{
public:
	vector<Object*> objects;

public:
	static MaterialLib matlib;	  // col·lecció de materials 

	Scene();

	void Init();
	void Render();
	void AddObject(Object*);

	Point Center();
	float RadiEscena();
};

#endif

