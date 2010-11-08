
#ifndef _SCENE_H_
#define _SCENE_H_

#include <QtOpenGL/qgl.h>

#include "Object.h"

using namespace std;

class Scene
{
    private:
        Box boundingBox;
        vector<Object> objects;
        int renderMode;
        int selectedObjectID;
        bool projective;

        int projectorTexture;

    public:
        static MaterialLib matlib;
    
    public:
        Scene();

        void Init();
        void Render();
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
        
        void setSelected(int id);
        void setDeselected();
        void redistributeSelectedObject(float incX, float incY);
        void deleteSelectedModel();
        void selectAll();

        void initProjectiveMode(bool b);
        void initializeProj();
        void setProjectorTexture(int textureID);
        void drawCube();
};

#endif
