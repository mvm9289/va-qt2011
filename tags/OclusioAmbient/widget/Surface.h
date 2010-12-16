#ifndef SURFACE_H
#define SURFACE_H

#include "Box.h"
#include "Ray.h"
#include "Point.h"
//#include "material.h"

class Surface; // forward declaration

/**
	Informacio d'una interseccio raig-superficie
**/
class SurfaceHitRecord 
{
 public:
	float t;				// The ray hits the surface at p = Ray.origin() + t*Ray.direction()
	Point p;				// Point of intersection
	Vector n;				// Normal of the surface at the intersection point 
	const Surface* surface;	// Intersected surface (makes sense for compound objects)
};


/**  
	Surface - Classe abstracta de la qual deriven Esfera, TriangleMesh, etc.
**/
class Surface 
{
public:

    /** 
		hit - Retorna cert si el raig intersecta la superficie. 
		En cas afirmatiu, retorna la primera interseccio.
		@param ray
		@param tmin: minimum hit parameter to be searched for
		@param tmax: maximum hit parameter to be searched for
	*/
    virtual bool hit(const Ray& r, float tmin, float tmax, SurfaceHitRecord& rec) const = 0;

    /** 
		Retorna cert si el raig intersecta la superficie. A diferència de l'anterior,
		pot acabar quan sap que hi ha alguna interseccio (no cal calcular la mes propera).
	**/
    virtual bool shadowHit(const Ray& r, float tmin, float tmax) const;

	/**
		Retorna la capsa minima contenidora de la superficie 
	*/
    //virtual Box boundingBox() const = 0;

	/**
		Acces al material
	**/
	//const Material* material(void) const;
	//void material(Material* material);


//protected:
	//Material* _material;

};

bool rayTriangleIntersection(const Ray& ray, const Point&p0, const Point& p1, const Point& p2, Point& intersect, float& t, float& up, float& vp, bool twoside);


#endif
