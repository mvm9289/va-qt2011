
#ifndef _BOX_H_
#define _BOX_H_

#include<vector>

#include "Point.h"
#include "Ray.h"

#ifdef min 
#undef min
#endif

#ifdef max 
#undef max
#endif

#define ALL_FACES -1

class SurfaceHitRecord;
class Face;

class Box
{
    public:
        Point minb, maxb;
        vector<Face> faces;
        
    private:
        void Render(double r, double g, double b);
        vector<float> planeCoefficients(Point A, Point B, Point C);

    public:
        Box(const Point& minimum=Point(), const Point& maximum=Point());
        void init(const Point& p);
        void update(const Point& p);
        void updateFaces();
        void Render();
        Point center();
        vector<float> renderRoom(float extra_size = 0.0, int face = ALL_FACES);
        float diagonal();
        bool hit(const Ray& r, float tmin, float tmax, SurfaceHitRecord& rec) const;
};

#endif
