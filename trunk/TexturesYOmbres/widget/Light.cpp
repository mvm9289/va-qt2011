
#include "Light.h"

#include <cmath>

Light::Light() {}

Light::Light(GLuint id, Point P)
{
    lightID = id;
    radius = firstRadius = 1;
    firstPos = P;
    latitude = 90;
    longitude = 90;
    updatePosition();

    float defaultLight[4] = {1, 1, 1, 1};
    glLightfv(id, GL_DIFFUSE, defaultLight);
    glLightfv(id, GL_SPECULAR, defaultLight);
}

Light::~Light() {}

void Light::updatePosition()
{
    pos.x = radius*sin((latitude*M_PI)/180.0)*cos((longitude*M_PI)/180.0) + firstPos.x;
    pos.y = radius*sin((latitude*M_PI)/180.0)*sin((longitude*M_PI)/180.0) + firstPos.y;
    pos.z = radius*cos((latitude*M_PI)/180.0) + firstPos.z;
}

void Light::setLatitude(float lat)
{
    latitude = lat/2.0;
    updatePosition();
}

void Light::setLongitude(float lon)
{
    longitude = lon/2.0;
    updatePosition();
}

void Light::setRadius(float rad)
{
    radius = rad/5.0*firstRadius;
    updatePosition();
}

Point Light::getPos()
{
    return pos;
}

void Light::setPos(Point P)
{
    firstPos = P;
    radius = firstRadius = 1;
}

void Light::sendPositionToGL()
{
    GLfloat position[4] = {pos.x, pos.y, pos.z, 1.0};
    glLightfv(lightID, GL_POSITION, position);
}
