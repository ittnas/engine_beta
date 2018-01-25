#include "parametric_sphere.hpp"

ParametricSphere::ParametricSphere(glm::vec3 center,GLfloat radius, GLuint n) {
  set_mesh();
}

Mesh * ParametricSphere::create_mesh() {
  pl = new ParametricPlot(plotting_function,glm::vec2(0,2*M_PI),glm::vec2(0,M_PI),21,21,GL_TRUE,GL_FALSE);
  return pl->get_mesh();
}

glm::vec3 ParametricSphere::plotting_function(GLfloat u,GLfloat r) {
  GLfloat x = radius*sin(r)*cos(u) + center.x;
  GLfloat y = radius*sin(r)*sin(u) + center.y;
  GLfloat z = radius*cos(r) + center.z;
  return glm::vec3(x,y,z);
}
