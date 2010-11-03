
#ifndef _FACE_H_
#define _FACE_H_

#include <vector>
#include "Vertex.h"

using namespace std;

class Face
{
    public:
        int material;
        vector<int> vertices;
        Vector normal;
        
    public:
        Face();
        Face(int i1, int i2, int i3, int i4=-1); 
        void computeNormal(vector<Vertex>& v);
};

#endif
