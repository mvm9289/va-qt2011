
#include <QtOpenGL/qgl.h>

#include "Box.h"

Box::Box(const Point& minimum, const Point& maximum):minb(minimum), maxb(maximum) {}

void Box::update(const Point& p)
{
    if (p.x < minb.x) minb.x=p.x;
    if (p.y < minb.y) minb.y=p.y;
    if (p.z < minb.z) minb.z=p.z;
    if (p.x > maxb.x) maxb.x=p.x;
    if (p.y > maxb.y) maxb.y=p.y;
    if (p.z > maxb.z) maxb.z=p.z;
}

void Box::init(const Point& p)
{
    minb = p;
    maxb = p;
}

void Box::Render()
{
    glLineWidth(1.0);
    Render(1.0, 1.0, 1.0);
}

void Box::Render(double r, double g, double b)
{
    glColor3f(r, g, b);
    glBegin(GL_LINES);
        glVertex3f(minb.x, minb.y, minb.z);
        glVertex3f(maxb.x, minb.y, minb.z);
    
        glVertex3f(maxb.x, minb.y, minb.z);
        glVertex3f(maxb.x, maxb.y, minb.z);
    
        glVertex3f(maxb.x, maxb.y, minb.z);
        glVertex3f(minb.x, maxb.y, minb.z);
    
        glVertex3f(minb.x, maxb.y, minb.z);
        glVertex3f(minb.x, minb.y, minb.z);
    
        glVertex3f(minb.x, minb.y, maxb.z);
        glVertex3f(maxb.x, minb.y, maxb.z);
    
        glVertex3f(maxb.x, minb.y, maxb.z);
        glVertex3f(maxb.x, maxb.y, maxb.z);
    
        glVertex3f(maxb.x, maxb.y, maxb.z);
        glVertex3f(minb.x, maxb.y, maxb.z);
        
        glVertex3f(minb.x, maxb.y, maxb.z);
        glVertex3f(minb.x, minb.y, maxb.z);
        
        glVertex3f(minb.x, maxb.y, minb.z);
        glVertex3f(minb.x, maxb.y, maxb.z);
        
        glVertex3f(maxb.x, maxb.y, minb.z);
        glVertex3f(maxb.x, maxb.y, maxb.z);
        
        glVertex3f(minb.x, minb.y, minb.z);
        glVertex3f(minb.x, minb.y, maxb.z);
        
        glVertex3f(maxb.x, minb.y, minb.z);
        glVertex3f(maxb.x, minb.y, maxb.z);
    glEnd();
}

Point Box::center()
{
    return (Point((maxb.x+minb.x)/2.,(maxb.y+minb.y)/2.,(maxb.z+minb.z)/2.));
}

void Box::renderRoom(float extra_size)
{
    Point min = minb;
    min.x -= extra_size;
    min.z -= extra_size;
    Point max = maxb;
    max.x += extra_size;
    max.y += extra_size;
    max.z += extra_size;

    Point A(min.x, max.y, max.z);
    Point B(min.x, max.y, min.z);
    Point C(max.x, max.y, min.z);
    Point D(max.x, max.y, max.z);
    Point E(min.x, min.y, max.z);
    Point F(min.x, min.y, min.z);
    Point G(max.x, min.y, min.z);
    Point H(max.x, min.y, max.z);

    glBegin(GL_QUADS);
        glNormal3f(1.0, 0.0, 0.0);
        glTexCoord3f(A.x, A.y, A.z);
        glVertex3f(A.x, A.y, A.z);
        glTexCoord3f(E.x, E.y, E.z);
        glVertex3f(E.x, E.y, E.z);
        glTexCoord3f(F.x, F.y, F.z);
        glVertex3f(F.x, F.y, F.z);
        glTexCoord3f(B.x, B.y, B.z);
        glVertex3f(B.x, B.y, B.z);

        glNormal3f(-1.0, 0.0, 0.0);
        glTexCoord3f(D.x, D.y, D.z);
        glVertex3f(D.x, D.y, D.z);
        glTexCoord3f(C.x, C.y, C.z);
        glVertex3f(C.x, C.y, C.z);
        glTexCoord3f(G.x, G.y, G.z);
        glVertex3f(G.x, G.y, G.z);
        glTexCoord3f(H.x, H.y, H.z);
        glVertex3f(H.x, H.y, H.z);

        glNormal3f(0.0, 0.0, 1.0);
        glTexCoord3f(B.x, B.y, B.z);
        glVertex3f(B.x, B.y, B.z);
        glTexCoord3f(F.x, F.y, F.z);
        glVertex3f(F.x, F.y, F.z);
        glTexCoord3f(G.x, G.y, G.z);
        glVertex3f(G.x, G.y, G.z);
        glTexCoord3f(C.x, C.y, C.z);
        glVertex3f(C.x, C.y, C.z);

        glNormal3f(0.0, 1.0, 0.0);
        glTexCoord3f(E.x, E.y, E.z);
        glVertex3f(E.x, E.y, E.z);
        glTexCoord3f(H.x, H.y, H.z);
        glVertex3f(H.x, H.y, H.z);
        glTexCoord3f(G.x, G.y, G.z);
        glVertex3f(G.x, G.y, G.z);
        glTexCoord3f(F.x, F.y, F.z);
        glVertex3f(F.x, F.y, F.z);

        glNormal3f(0.0, -1.0, 0.0);
        glTexCoord3f(A.x, A.y, A.z);
        glVertex3f(A.x, A.y, A.z);
        glTexCoord3f(B.x, B.y, B.z);
        glVertex3f(B.x, B.y, B.z);
        glTexCoord3f(C.x, C.y, C.z);
        glVertex3f(C.x, C.y, C.z);
        glTexCoord3f(D.x, D.y, D.z);
        glVertex3f(D.x, D.y, D.z);

        glNormal3f(0.0, 0.0, -1.0);
        glTexCoord3f(A.x, A.y, A.z);
        glVertex3f(A.x, A.y, A.z);
        glTexCoord3f(D.x, D.y, D.z);
        glVertex3f(D.x, D.y, D.z);
        glTexCoord3f(H.x, H.y, H.z);
        glVertex3f(H.x, H.y, H.z);
        glTexCoord3f(E.x, E.y, E.z);
        glVertex3f(E.x, E.y, E.z);
    glEnd();
}
