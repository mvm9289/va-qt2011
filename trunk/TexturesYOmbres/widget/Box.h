
#ifndef _BOX_H_
#define _BOX_H_

#include "Point.h"

#ifdef min 
#undef min
#endif

#ifdef max 
#undef max
#endif

class Box
{
    public:
        Point minb, maxb;
        
    private:
        void Render(double r, double g, double b);

    public:
        Box(const Point& minimum=Point(), const Point& maximum=Point());
        void init(const Point& p);
        void update(const Point& p);
        void Render();
        Point center();
        void renderRoom(int face, float scale);
};

#endif
