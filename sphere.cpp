#include "sphere.hpp"

Sphere::Sphere(glm::vec3 center, GLfloat radius, GLuint n) : Shape() {
  set_mesh();
}

Mesh * Sphere::create_mesh() {
  //TODO
  return NULL;
}
