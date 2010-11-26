#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <QtOpenGL/qgl.h>

#include <vector>
#include "Vertex.h"
#include "Box.h"
#include "Face.h"
#include "MaterialLib.h"
#include "Surface.h"

using namespace std;

// Object - permet representar un objecte 3D
class Object : public Surface
{
 public:
  Object(std::string);
  ~Object();

  // llegeix un model en format OBJ
  void readObj(const char* filename, MaterialLib& matlib);

  // calcula la capsa del model
  void updateBoundingBox();

  // retorna la capsa del model (previament s'ha d'haver calculat)
  Box boundingBox() const;

  // calcula normals per vertex
  void updateVertexNormals(); 

  //Inicialitza objectes d'openGL
  void initGL();

  void render();
  void updateNormals();

  virtual bool hit(const Ray& r, float tmin, float tmax, SurfaceHitRecord& rec) const;

 private:
  void make_face ( char **words, int nwords, int material );
  void clear ();

 public:
  vector<Vertex> vertices;  // vector amb els vertexs de l'objecte
  vector<Face> faces;       // vector amb les cares de l'objecte

 private: 
  std::string name;
  Box _boundingBox;   // caixa contenidora de l'objecte.
};


#endif
