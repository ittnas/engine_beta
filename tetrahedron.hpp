#ifndef TETRAHEDRON_HPP
#define TETRAHEDRON_HPP

#include "shape.hpp"

class Tetrahedron : public Shape {
private:
  GLfloat size;
  const GLfloat SQRT2 = 0.7071067;
protected:
  virtual Mesh * create_mesh();
public:
  virtual ~Tetrahedron() {}
  Tetrahedron(GLfloat size = 1.0);
};

#endif
