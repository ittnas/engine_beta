#ifndef AABB_HPP
#define AABB_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>

// THIS IS NOT USED. THE REAL IMPLEMENTATION IS IN bounding_geometry.cpp
class Aabb {
private:
  glm::vec4 vertices[8];
  GLfloat min[3];
  GLfloat max[3];
  
public:
  Aabb(GLfloat * min, GLfloat * max);
  //Aabb(glm::vec4 * vertices);
  ~Aabb() {}

  glm::vec4 * get_vertices() {return vertices;}
  GLfloat * get_min() {return min;}
  GLfloat * get_max() {return max;}
  void set_min_max(GLfloat * min, GLfloat * max);
  
};

#endif
