#ifndef CUBOID_HPP
#define CUBOID_HPP

#include <GL/glew.h>

#include "shape.hpp"

// TODO nothing is done.
class Cuboid : public Shape {
private:
  GLfloat height,width,length;
protected:
  virtual Mesh * create_mesh();
public:
  virtual ~Cuboid() {}
  Cuboid(GLfloat height,GLfloat width,GLfloat length);
};

#endif
