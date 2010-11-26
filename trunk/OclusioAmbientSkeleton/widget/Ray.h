/**

		Ray.h
		
		A class for representing 3D rays as a pair (point, direction vector)
	
*/

#ifndef _RAY_H__
#define _RAY_H__

#include <Point.h>
#include <iostream>
using namespace std;

class Ray
{
public:
	
	/**
	Default constructor (does nothing).
	*/
	Ray();

	/**
	Constructor from two points.
	The ray is oriented from the first point to the second point.
	Both points must be different.
	*/
	Ray(const Point&, const Point&);

	/**
	Constructor from a point and a (non-null) direction vector.
	*/
	Ray(const Point&, const Vector&);

	/**
	Return a unit vector in the direction of the ray
	*/
	Vector vector(void) const;

	/** 
	Return ray's origin
	*/
	Point origin(void) const;	
	
public:

	Point p;	// origin
	Vector v;	// vector along the line (not normalized)

};


#endif

