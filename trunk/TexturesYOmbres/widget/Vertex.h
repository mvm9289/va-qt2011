#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "Point.h"

// Vertex - representa un vertex d'un objecte 3D
class Vertex
{
 private:
    int normals;
    
 public:
	Vertex(const Point& coordenades);

	// atributs
	Point coord;		// coordenades del vertex
	Vector normal;
};



#endif
