#ifndef CUBE_HPP
#define CUBE_HPP

#include <GL/glew.h>

#include "shape.hpp"

class Cube : public Shape {
private:
  GLfloat size;
protected:
  virtual Mesh * create_mesh();
public:
  virtual ~Cube() {}
  Cube(GLfloat size);
};

#endif
