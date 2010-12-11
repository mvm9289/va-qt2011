
#ifndef _POINT_H_
#define _POINT_H_

#include <iostream>

using namespace std;

class Point
{
    public:
        float x, y, z;
        
    public:
        Point(float px=0., float py=0., float pz=0.);
};

class Vector
{
    public:
        float x, y, z;
        
    public:
        Vector (float vx=0., float vy=0., float vz=0.);
        explicit Vector(const Point &p);
        float  operator[](int i) const;
        float& operator[](int i); 
        float length() const;
        float squaredLength() const;
        void normalize();
};

Point   operator+(const Point&, const Point&);
Point&  operator+=(Point&, const Point&);
Vector operator-(const Point&, const Point&);
Point  operator+(const Point&, const Vector &v);
Point& operator+=(Point&, const Vector &v);
Point  operator-(const Point&, const Vector&);
Point& operator-=(Point&, const Vector &v);
Vector   operator+(const Vector&, const Vector&);
Vector&  operator+=(Vector&, const Vector&);
Vector operator-(const Vector&, const Vector&);
Vector& operator-=(Vector&, const Vector &v);
Point operator* (const Point&, float f);
Point operator* (float f, const Point&);
Point operator/ (const Point&, float f);
Point&operator/=(Point&, float f);
Vector operator* (const Vector&, float f);
Vector operator* (float f, const Vector&);
float operator* (const Vector&, const Vector&);
Vector operator^ (const Vector&, const Vector&);
Vector operator/ (const Vector&, float f);
Vector&operator/=(Vector&, float f);

ostream& operator<< (ostream &os, const Point &p);
ostream& operator<< (ostream &os, const Vector &p);

#endif
