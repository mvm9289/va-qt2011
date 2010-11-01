#include "Face.h"
#include <cmath>

Face::Face() 
{ 
  normal.x=0.0;
  normal.y=0.0;
  normal.z=0.0;
}

Face::Face(int i1, int i2, int i3, int i4)
{
  vertices.push_back(i1);
  vertices.push_back(i2);
  vertices.push_back(i3);
  if (i4!=-1) vertices.push_back(i4);
  normal.x=0.0;
  normal.y=0.0;
  normal.z=0.0;
}

void Face::updateNormal(vector<Vertex> &v)
{
  /*int j;
  normal.x=0.0;
  normal.y=0.0;
  normal.z=0.0;
  int n = vertices.size();
  for (int i=0; i<n; i++)
  {
    j = (i+1)%n;

    normal.x += ((v[vertices[i]].coord.z+v[vertices[j]].coord.z)*
                 (v[vertices[i]].coord.y-v[vertices[j]].coord.y));
    normal.y += ((v[vertices[i]].coord.x+v[vertices[j]].coord.x)*
                 (v[vertices[i]].coord.z-v[vertices[j]].coord.z));
    normal.z += ((v[vertices[i]].coord.y+v[vertices[j]].coord.y)*
                 (v[vertices[i]].coord.x-v[vertices[j]].coord.x));
  }

  normal.x *= 0.5;
  normal.y *= 0.5;
  normal.z *= 0.5;
  normal.normalize();*/
  
  Point a = v[vertices[0]].coord;
  Point b = v[vertices[1]].coord;
  Point c = v[vertices[2]].coord;
  
  Vector v1 = b - a;
  Vector v2 = c - a;
  
  normal.x = (v1.y*v2.z) - (v1.z*v2.y);
  normal.y = -((v2.z*v1.x) - (v2.x*v1.z));
  normal.z = (v1.x*v2.y) - (v1.y*v2.x);
  
  normal.normalize();
  
  int n = vertices.size();
  for (int i = 0; i < n; i++)
  {
	v[vertices[i]].normal.x += normal.x;
	v[vertices[i]].normal.y += normal.y;
	v[vertices[i]].normal.z += normal.z;
	
	v[vertices[i]].normal.normalize();
  }
}
