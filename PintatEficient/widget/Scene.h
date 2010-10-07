#ifndef _Scene_H_
#define _Scene_H_
#include <QtOpenGL/qgl.h>

#include "Object.h"

using namespace std;

class Scene
{
private:
	vector<Object> objects;
	int renderMode;

public:
	static MaterialLib matlib;	  // col·lecció de materials 

	Scene();

	void ChangeRenderMode(int mode);

	void Init();
	void Render();
	void AddObject(Object &);
	void OpenModel(const char* filename);

	Point Center();
	float RadiEscena();
};

#endif

