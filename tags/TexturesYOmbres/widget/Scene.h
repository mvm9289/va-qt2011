
#ifndef _SCENE_H_
#define _SCENE_H_

#include <QtOpenGL/qgl.h>

#include "Object.h"
#include "Light.h"

#define ALL_OBJECTS -10
#define NONE_OBJECT -1
#define EXTRA_SIZE_OF_ROOM 10

using namespace std;

class Scene
{
    private:
        Box boundingBox;
        vector<Object> objects;
        Light light;
        int renderMode;
        int selectedObjectID;
        bool shadows;

    public:
        static MaterialLib matlib;
    
    private:
        void renderObjects(bool projector = false);
        void renderShadows();
    
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
        
        void setShadows();

        void setLightRadius(float rad);
        void setLightLongitude(float lon);
        void setLightLatitude(float lat);
};

#endif
