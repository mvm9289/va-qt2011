
#ifndef _BOX_H_
#define _BOX_H_

#include<vector>

#include "Point.h"

using namespace std;

#ifdef min 
#undef min
#endif

#ifdef max 
#undef max
#endif

#define ALL_FACES -1

class Box
{
    public:
        Point minb, maxb;
        
    private:
        void Render(double r, double g, double b);
        vector<float> planeCoefficients(Point A, Point B, Point C);

    public:
        Box(const Point& minimum=Point(), const Point& maximum=Point());
        void init(const Point& p);
        void update(const Point& p);
        void Render();
        Point center();
        vector<float> renderRoom(float extra_size = 0.0, int face = ALL_FACES);
};

#endif
