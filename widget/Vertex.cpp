#include "Vertex.h"

Vertex::Vertex(const Point& coordenades)
: coord(coordenades)
{}

void Vertex::computeNormal(Vector faceNormal)
{
  normal += faceNormal;
}


