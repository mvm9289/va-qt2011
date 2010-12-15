
#ifndef __ACCELERATOR_H__
#define __ACCELERATOR_H__

#include "Face.h"
#include "Vertex.h"
#include "Box.h"
#include "Ray.h"

#include <vector>

class Object;

enum Axis
{
  X, Y, Z
};

class Accelerator 
{
private:
  Box box;
  Accelerator* subNode1;
  Accelerator* subNode2;
  Object* owner;
  vector<int> faces;
private:
	void createBox();
	void createSubnodes();
	Axis findAxis(float& limit);
	void axisPartition(vector<int>& f1, vector<int>& f2);
public:
  Accelerator(Object* obj, vector<int> f);
  ~Accelerator();
  void render();
  bool shadowHit(const Ray& r, float tmin, float tmax);
  bool hit(const Ray& r, float tmin, float tmax, SurfaceHitRecord& rec);
};

#endif