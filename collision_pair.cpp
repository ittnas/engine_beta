#include "collision_pair.hpp"

CollisionPair::CollisionPair(CollisionObject * co1, CollisionObject * co2, glm::vec3 point_of_intersection, glm::vec3 normal_of_intersection, GLfloat depth_of_intersection) : co1(co1), co2(co2), point_of_intersection(point_of_intersection), normal_of_intersection(normal_of_intersection), depth_of_intersection(depth_of_intersection) {
  
}
/* WTF, you cannot return just value
std::pair<CollisionObject *, CollisionObject *> CollisionPair::get_colliding_objects() const {
  return std::make_pair(co1,co2);
}
*/
