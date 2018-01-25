#include "parametric_surface_generator.hpp"

ParametricSphereGenerator::ParametricSphereGenerator(glm::vec3 center, GLfloat radius) : ParametricSurfaceGenerator(), center(center), radius(radius) {}

glm::vec3 ParametricSphereGenerator::get_value_at(GLfloat u, GLfloat r) const {
  GLfloat x = radius*sin(r)*cos(u) + center.x;
  GLfloat y = radius*sin(r)*sin(u) + center.y;
  GLfloat z = radius*cos(r) + center.z;
  return glm::vec3(x,y,z);
}

ParametricSurfaceGeneratorFromFunction::ParametricSurfaceGeneratorFromFunction(ParametricSurface function) : function(function) {
  
}

glm::vec3 ParametricSurfaceGeneratorFromFunction::get_value_at(GLfloat u, GLfloat r) const {
  return function(u,r);
}
