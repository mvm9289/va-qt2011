
#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "Point.h"

class Vertex
{
    public:
        Point coord;
        Vector normal;
    
    public:
        Vertex(const Point& coordenades);
};

#endif
