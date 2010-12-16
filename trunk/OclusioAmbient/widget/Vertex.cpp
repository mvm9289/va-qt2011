
#include "Vertex.h"
#include "Directions.h"

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
