#ifndef BASIC_GEOMETRY_SHAPES_HPP
#define BASIC_GEOMETRY_SHAPES_HPP

#include "shape.hpp"

class RayShape : public Shape {
private:
  glm::vec3 a;
  glm::vec3 d;
public:
  RayShape(const glm::vec3 & a, const glm::vec3 & d);
  virtual ~RayShape() {}
protected:
  virtual Mesh * create_mesh();
};

class PointCloud : public Shape {
private:
  std::vector<glm::vec4> points;
public:
  PointCloud(const std::vector<glm::vec4> & points);
  virtual ~PointCloud() {}
protected:
  virtual Mesh * create_mesh();
};

#endif
