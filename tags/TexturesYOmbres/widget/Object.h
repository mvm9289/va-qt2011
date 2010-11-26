
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <vector>
#include <cstdio>

#include "Point.h"
#include "Vertex.h"
#include "Face.h"
#include "Box.h"
#include "MaterialLib.h"

#define IMMEDIATE 0
#define DISPLAY_LIST 1
#define VERTEX_ARRAYS 2

using namespace std;

class Object
{
    private:
        std::string name;
        Box _boundingBox;
        Point pos;
        Point center;
        
        int DLindex;
        int triangles;
        int quads;
        GLfloat *vertices2;
        GLfloat *normals;
        GLfloat *colors;
        GLfloat *texCoords;
        GLuint *vertexTriangles;
        GLuint *vertexQuads;
    
        int texture;
        unsigned int wrapS;
        unsigned int wrapT;
    
        bool selected;
    
    public:
        vector<Vertex> vertices;
        vector<Face> faces;

    private:
        void make_face(char **words, int nwords, int material);
        void clear();
    
        void createDisplayList();
        void createVertexArrays();
        void recreateTexCoordArray();
        inline void immediateRender();
        inline void vertexArraysRender();
        inline void selectedRender();
    
        void projectorRender();
    
    public:
        Object(std::string);
        ~Object();
        void readObj(const char* filename, MaterialLib& matlib);
        void computeBoundingBox();
        Box boundingBox();
        
        Point getPos();
        void setPos(Point p);
    
        vector<int> numTrianglesQuads();
        
        void initGL();
        void render(int mode, bool projector = false);
        void updateNormals();
        void setTexture(int textureID);
        void repeatWrapS(int sWrap);
        void repeatWrapT(int tWrap);
    
        void setSelected(bool _selected);
};

#endif