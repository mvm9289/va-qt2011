
#include "Vertex.h"
#include "Directions.h"

#define EPSILON_ 0.707106781 //cosinus(45)

Vertex::Vertex(const Point& coordenades):coord(coordenades), /*corner(false),*/ occlusion(1), obscurance(1)
{
    normal.x = 0;
    normal.y = 0;
    normal.z = 0;
}

vector<Vector> Vertex::rays(int numDir)
{
    vector<Vector> result;
    randomDirections(normal, numDir, result);

    return result;
}

//~ void Vertex::cornerTest()
//~ {
    //~ int n = facesNormals.size();
    //~ for (int i = 0; i < n && !corner; i++)
        //~ for (int j = 0; j < n && !corner; j++)
            //~ if (i != j && facesNormals[i]*facesNormals[j] < EPSILON_) corner = true;
//~ }
