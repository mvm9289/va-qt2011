#ifndef _FACE_H_
#define _FACE_H_

#include <vector>
#include "Vertex.h"
#include "Surface.h"

using namespace std;

class Object;

// Face - representa una cara d'un objecte 3D
class Face : public Surface
{
 public:
	Face(Object* owner);

	// constructor a partir de 3 o 4 indexs a vertex
	Face(Object* owner, int i1, int i2, int i3, int i4=-1); 

	void updateNormal();

	virtual bool hit(const Ray& r, float tmin, float tmax, SurfaceHitRecord& rec) const;


	// atributs
	int material;	       // ID del material (index del material)
	vector<int> vertices;  // vector amb els indexs dels vertexs de la cara

    Vector normal;
	Object* object;	// apuntador a l'objecte que conté la cara
    
};



#endif
