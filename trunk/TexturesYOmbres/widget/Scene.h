
#ifndef _SCENE_H_
#define _SCENE_H_

#include <QtOpenGL/qgl.h>

#include "Object.h"

using namespace std;

class Scene
{
    private:
        vector<Object> objects;
        int renderMode;

    public:
        static MaterialLib matlib;
    
    public:
        Scene();

        void Init();
        void Render();
        void AddObject(Object &);
    
        Point center();
        float radius();
    
	vector<int> numTrianglesQuads_Model();
    
        void ChangeRenderMode(int mode);
        void OpenModel(const char* filename);
        void setTexture(int textureID);
};

#endif
