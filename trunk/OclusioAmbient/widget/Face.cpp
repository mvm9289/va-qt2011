
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

bool rayTriangleIntersection(const Ray& ray, const Point&p0, const Point& p1, const Point& p2, Point& intersect, float& t, float& up, float& vp, bool twoside)
{
    const double EPSILON = 0.0001;

    /* find vectors for two edges sharing vert0 */
    Vector edge1=p1-p0;
    Vector edge2=p2-p0;

    /* begin calculating determinant - also used to calculate U parameter */
    Vector pvec = ray.v ^ edge2;

    /* if determinant is near zero, ray lies in plane of triangle */
    double det = edge1*pvec;

    if (twoside)
    {
        if (det < EPSILON) return false;

        /* calculate distance from vert0 to ray origin */
        Vector tvec = ray.p - p0;

        /* calculate U parameter and test bounds */
        double up =tvec*pvec;
        if (up < 0.0 || up > det)  return false;

        /* prepare to test V parameter */
        Vector qvec= tvec ^ edge1;

        /* calculate V parameter and test bounds */
        vp = ray.v*qvec;
        if (vp < 0.0 || up + vp > det) return false;

        /* calculate t, scale parameters, ray intersects triangle */
        t = edge2 * qvec;
        double inv_det = (float)1.0 / det;
        t *= float(inv_det);
        up *= float(inv_det);
        vp *= float(inv_det);
    }
    else
    {
        if (det > -EPSILON && det < EPSILON) return false;

        double inv_det = (double)1.0 / det;

        /* calculate distance from vert0 to ray origin */
        Vector tvec= ray.p - p0;

        /* calculate U parameter and test bounds */
        up = (tvec*pvec) * float(inv_det);
        if (up < 0.0f || up > 1.0f) return false;

        /* prepare to test V parameter */
        Vector qvec= tvec ^ edge1;

        /* calculate V parameter and test bounds */
        vp = (ray.v*qvec) * float(inv_det);
        if (vp < 0.0 || up + vp > 1.0) return false;

        /* calculate t, ray intersects triangle */
        t = (edge2*qvec) * float(inv_det);
    }

    if (t<0) return false;  // CAG: consideramos una semirecta, no una recta

    // Otra forma de calcular la interseccion seria:  (1-u-v)*p0 + u*p1 + v*p2
    intersect = ray.p+float(t)*ray.v;

    return (true);
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
