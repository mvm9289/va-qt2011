
#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <QtOpenGL/qgl.h>

#include "Point.h"

class Light
{
    private:
        GLuint lightID;
        float longitude, latitude, radius, firstRadius;
        Point pos;
    
    private:
        void updatePosition();
    
    public:
        Light();
        Light(GLuint id, float rad);
        ~Light();
    
        void setLatitude(float lat);
        void setLongitude(float lon);
        void setRadius(float rad);
        Point getPos();
        void sendPositionToGL();
};

#endif