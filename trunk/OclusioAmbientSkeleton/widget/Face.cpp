#include "Face.h"
#include "Object.h"
#include <cmath>

Face::Face(Object* owner) 
{ 
	object = owner;
  normal.x=0.0;
  normal.y=0.0;
  normal.z=0.0;
}

Face::Face(Object* owner, int i1, int i2, int i3, int i4)
{
	object = owner;
  vertices.push_back(i1);
  vertices.push_back(i2);
  vertices.push_back(i3);
  if (i4!=-1) vertices.push_back(i4);
  normal.x=0.0;
  normal.y=0.0;
  normal.z=0.0;
}

void Face::updateNormal()
{

	vector<Vertex> &v = object->vertices;

  int j;
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

  //cout << "N:" << normal << endl;
  normal.normalize();
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


bool Face::hit(const Ray& r, float tmin, float tmax, SurfaceHitRecord& rec, const vector<Vertex>& v) const
{
	// PRAC3
	// aquesta l'heu d'implementar vosaltres
	exit(0);
	return false;
}
