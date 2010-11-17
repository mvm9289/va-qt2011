
#include "Light.h"

#include <cmath>

Light::Light() {}

Light::Light(GLuint id, Point P, float r)
{
    lightID = id;
    radius = firstRadius = r;
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
    pos.x = radius*cos((latitude*M_PI)/180.0)*sin((longitude*M_PI)/180.0) + firstPos.x;
    pos.y = radius*sin((latitude*M_PI)/180.0) + firstPos.y;
    pos.z = radius*cos((latitude*M_PI)/180.0)*cos((longitude*M_PI)/180.0) + firstPos.z;
}

void Light::setLatitude(float lat)
{
    latitude = lat;
    updatePosition();
}

void Light::setLongitude(float lon)
{
    longitude = lon;
    updatePosition();
}

void Light::setRadius(float rad)
{
    radius = rad*firstRadius;
    updatePosition();
}

Point Light::getPos()
{
    return pos;
}

void Light::sendPositionToGL()
{
    GLfloat position[4] = {pos.x, pos.y, pos.z, 1.0};
    glLightfv(lightID, GL_POSITION, position);
}
