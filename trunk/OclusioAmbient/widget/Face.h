
#ifndef _FACE_H_
#define _FACE_H_

#include <vector>
#include "Vertex.h"
#include "Surface.h"

class Object;

class Face:public Surface
{
    public:
        Object* owner;
        int material;
        vector<int> vertices;
        Vector normal;
        bool isBoxFace;
        Point A, B, C, D;
       
    public:
        Face(Object* obj);
        Face(Object* obj, int i1, int i2, int i3, int i4=-1); 
        Face(Point a, Point b, Point c, Point d);
        void computeNormal(vector<Vertex>& verts);
        virtual bool hit(const Ray& r, float tmin, float tmax, SurfaceHitRecord& rec) const;
};

#endif
