
#ifndef __ACCELERATOR_H__
#define __ACCELERATOR_H__

#include <vector>
#include "Face.h"
#include "Box.h"
#include "Ray.h"
#include "Surface.h"

class Accelerator
{
    private:
        Box box;
        vector<Accelerator*> subnodes;
        vector<Face> faces;
    
    private:
        void createBox();
        void createSubnodes();
        bool createSubnodes(Box b1);
        bool isInterior(Box b, Face f);
    public:
        Accelerator(const vector<Face>& f);
        ~Accelerator();
        void render();
        bool hit(const Ray& r, float tmin, float tmax, SurfaceHitRecord& rec) const;
        bool hitMinDist(const Ray& r, float tmin, float tmax, SurfaceHitRecord& rec) const;
};

#endif
