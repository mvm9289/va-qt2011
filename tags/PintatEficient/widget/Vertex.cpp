#include "Vertex.h"

Vertex::Vertex(const Point& coordenades)
: coord(coordenades),normals(0), normal(0)
{}

void Vertex::computeNormal(Vector faceNormal)
{
  normal += faceNormal;
  normals++;
}

void Vertex::normalize()
{
  normal /= normals;
  normal.normalize();
}


