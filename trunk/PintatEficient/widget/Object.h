#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <QtOpenGL/qgl.h>

#include <vector>
#include "Vertex.h"
#include "Box.h"
#include "Face.h"
#include "MaterialLib.h"

using namespace std;

// Object - permet representar un objecte 3D
class Object
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

  //Inicialitza objectes d'openGL
  void initGL();

  void render(int mode);
  void updateNormals();

	vector<int> numTrianglesQuads();

 private:
  void make_face ( char **words, int nwords, int material );
  void clear ();
 
  void createDisplayList();
  void createVertexArrays();


 public:
  vector<Vertex> vertices;  // vector amb els vertexs de l'objecte
  vector<Face> faces;       // vector amb les cares de l'objecte

 private: 
  std::string name;
  Box _boundingBox;   // caixa contenidora de l'objecte.
 
  GLuint DLindex;
  
  int triangles;
  int quads;
  GLfloat *vertices2;
  GLfloat *normals;
  GLfloat *colors;
  GLuint *vertexTriangles;
  GLuint *vertexQuads;
};


#endif
