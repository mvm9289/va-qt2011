
#include "Light.h"

#include <cmath>

Light::Light() {}

Light::Light(GLuint id, float rad)
{
    lightID = id;
    radius = firstRadius = rad;
    latitude = 90;
    longitude = 0;
    updatePosition();
}

Light::~Light() {}

void Light::updatePosition()
{
    pos.x = radius*sin((latitude*M_PI)/180.0)*cos((longitude*M_PI)/180.0);
    pos.y = radius*sin((latitude*M_PI)/180.0)*sin((longitude*M_PI)/180.0);
    pos.z = radius*cos((latitude*M_PI)/180.0);
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
    radius = ((rad/130) + 1)*firstRadius;
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
