#ifndef COLLISION_PAIR_HPP
#define COLLISION_PAIR_HPP

#include <glm/glm.hpp>
#include <GL/glew.h>
//#include <utility>

//#include "collision_object.hpp"
class CollisionObject;

class CollisionPair {
private:
  CollisionObject * co1;
  CollisionObject * co2;
  glm::vec3 point_of_intersection;
  glm::vec3 normal_of_intersection;
  GLfloat depth_of_intersection;
public:
  CollisionPair(CollisionObject * co1, CollisionObject * co2, glm::vec3 point_of_intersection = glm::vec3(0,0,0),glm::vec3 normal_of_intersection = glm::vec3(0,0,0),GLfloat depth_of_intersection = 0);
  //std::pair<CollisionObject *,CollisionObject *> get_colliding_objects() const;
  CollisionObject * get_first_collision_object() const {return co1;}
  CollisionObject * get_second_collision_object() const {return co2;}
  glm::vec3 get_point_of_intersection() const {return point_of_intersection;}
  glm::vec3 get_normal_of_intersection() const {return normal_of_intersection;}
  GLfloat get_depth_of_intersection() const {return depth_of_intersection;}
  ~CollisionPair() {}
};
#endif
