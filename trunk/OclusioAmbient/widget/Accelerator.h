
#ifndef __ACCELERATOR_H__
#define __ACCELERATOR_H__

#include <vector>
#include "Face.h"
#include "Box.h"
#include "Ray.h"
#include "Surface.h"

class Object;

enum Axis
{
    X, Y, Z
};

class Accelerator
{
    private:
        Box box;
        Accelerator* subNode1;
        Accelerator* subNode2;
        Object* owner;
        vector<int> faces;
    
    private:
        void createBox();
        void createSubnodes();
        bool createSubnodes(Axis axis, float limit);
        bool atLeft(Axis axis, float limit, Face f);
    public:
        Accelerator(Object* obj, vector<int> f);
        ~Accelerator();
        void render();
        bool hit(const Ray& r, float tmin, float tmax, SurfaceHitRecord& rec) const;
        bool hitMinDist(const Ray& r, float tmin, float tmax, SurfaceHitRecord& rec) const;
};

#endif
