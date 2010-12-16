
#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "Point.h"

#include <vector>

class Vertex
{
    public:
        Point coord;
        Vector normal;
        //~ bool corner;
        //~ vector<Vector> facesNormals;
        float occlusion;
        float obscurance;
    
    public:
        Vertex(const Point& coordenades);
        vector<Vector> rays(int numDir);
        //~ void cornerTest();
};

#endif
