#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "shape.hpp"

class Triangle : public Shape {
private:
  glm::vec3 v[3];
protected:
  virtual Mesh * create_mesh();
public:
  virtual ~Triangle() {}
  Triangle(const glm::vec3 & v0,const glm::vec3 & v1,const glm::vec3 & v2);
};

#endif
