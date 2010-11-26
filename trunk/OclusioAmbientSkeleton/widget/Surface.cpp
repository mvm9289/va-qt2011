#include "Surface.h"

bool Surface::shadowHit(const Ray& r, float tmin, float tmax) const
{
	SurfaceHitRecord rec;
	return hit(r, tmin, tmax, rec);
}

/*
const Material* Surface::material(void) const
{
	return _material;
}


void Surface::material(Material* material)
{
	_material=material;
}

*/
