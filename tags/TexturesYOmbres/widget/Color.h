
#ifndef _COLOR_H_
#define _COLOR_H_

#include <iostream>
using namespace std;

class Color
{
    public:
        float r, g, b, a;
        
    public:
        Color(float red=0., float green=0., float blue=0., float alfa=1.);
};

ostream& operator<< (ostream &os, const Color &c);

#endif
