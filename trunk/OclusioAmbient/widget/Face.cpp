
#include <cmath>

#include "Face.h"
#include "Object.h"

Face::Face(Object* obj) 
{
    owner = obj;
    normal.x=0.0;
    normal.y=0.0;
    normal.z=0.0;
}

Face::Face(Object* obj, int i1, int i2, int i3, int i4)
{
    owner = obj;
    vertices.push_back(i1);
    vertices.push_back(i2);
    vertices.push_back(i3);
    if (i4!=-1) vertices.push_back(i4);
    normal.x=0.0;
    normal.y=0.0;
    normal.z=0.0;
}

void Face::computeNormal(vector<Vertex> &v)
{
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

bool Face::hit(const Ray& r, float tmin, float tmax, SurfaceHitRecord& rec) const
{
    float up, vp;
    rec.n = normal;
    vector<Vertex> v = owner->vertices;
    
    bool intersect = rayTriangleIntersection(r, v[vertices[0]].coord, v[vertices[1]].coord, v[vertices[2]].coord, rec.p, rec.t, up, vp, true);
    if (!intersect && vertices.size() > 3) intersect = rayTriangleIntersection(r, v[vertices[1]].coord, v[vertices[2]].coord, v[vertices[3]].coord, rec.p, rec.t, up, vp, true);
    
    return intersect && rec.t > tmin && rec.t < tmax;
}
