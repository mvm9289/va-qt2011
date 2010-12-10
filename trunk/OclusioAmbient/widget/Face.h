
#ifndef _FACE_H_
#define _FACE_H_

#include <vector>
#include "Vertex.h"
#include "Surface.h"

using namespace std;

class Object;

class Face:public Surface
{
    public:
        int material;
        vector<int> vertices;
        Vector normal;
        Object* owner;
       
    public:
        Face(Object* owner);
        Face(Object* owner, int i1, int i2, int i3, int i4=-1); 
        void computeNormal(vector<Vertex>& v);
        virtual bool hit(const Ray& r, float tmin, float tmax, SurfaceHitRecord& rec) const;
};

#endif
