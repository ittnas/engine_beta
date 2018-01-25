#include <cstring>

#include "aabb.hpp"


Aabb::Aabb(GLfloat * min, GLfloat * max) {
  set_min_max(min,max);
}

void Aabb::set_min_max(GLfloat * min, GLfloat * max) {
  vertices[0] = glm::vec4(min[0],min[1],min[2],1.0);
  vertices[1] = glm::vec4(max[0],min[1],min[2],1.0);
  vertices[2] = glm::vec4(max[0],min[1],max[2],1.0);
  vertices[3] = glm::vec4(min[0],min[1],max[2],1.0);

  vertices[4] = glm::vec4(min[0],max[1],min[2],1.0);
  vertices[5] = glm::vec4(max[0],max[1],min[2],1.0);
  vertices[6] = glm::vec4(max[0],max[1],max[2],1.0);
  vertices[7] = glm::vec4(min[0],max[1],max[2],1.0);

  this->min[0] = min[0];
  this->min[1] = min[1];
  this->min[2] = min[2];

  this->max[0] = max[0];
  this->max[1] = max[1];
  this->max[2] = max[2];
}

/*
Aabb::Aabb(glm::vec4 * vertices) {
  memcpy(vertices,this->vertices,sizeof(glm::vec4)*8);
}
*/
