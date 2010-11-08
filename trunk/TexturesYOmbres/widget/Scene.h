
#ifndef _SCENE_H_
#define _SCENE_H_

#include <QtOpenGL/qgl.h>

#include "Object.h"

#define ALL_OBJECTS -10
#define NONE_OBJECT -1

using namespace std;

class Scene
{
    private:
        Box boundingBox;
        vector<Object> objects;
        int renderMode;
        int selectedObjectID;
        bool projective;

    public:
        static MaterialLib matlib;
    
    public:
        Scene();

        void Init();
        void Render(bool projector = false);
        void AddObject(Object &);
    
        void updateBoundingBox();
        Point center();
        float radius();
    
        vector<int> numTrianglesQuads();
    
        void ChangeRenderMode(int mode);
        void OpenModel(const char* filename);
        void setTexture(int textureID);
        void repeatWrapS(int sWrap);
        void repeatWrapT(int tWrap);
        
        void setSelected(int id, bool selected);
        void redistributeSelectedObject(Point inc);
        void deleteSelectedModel();
};

#endif
