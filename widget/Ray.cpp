
#include "Ray.h"

Ray::Ray() {}


Ray::Ray(const Point&p1, const Point&p2)
{
	p=p1;			// punto de paso
	v=(p2-p1);		// vector director: de p1 a p2
	v.normalize();	// el vector se mantiene normalizado
}


Ray::Ray(const Point&paso, const Vector& vdir)
{
	p=paso;
	v=vdir;
	v.normalize();	// el vector se mantiene normalizado
}




Vector Ray::vector(void) const
{
	return v;
}


Point Ray::origin(void) const
{
	return p;
}

