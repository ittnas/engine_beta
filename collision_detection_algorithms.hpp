#ifndef COLLISION_DETECTION_ALGORITHMS_HPP
#define COLLISION_DETECTION_ALGORITHMS_HPP

#include <tuple>
#include "bounding_geometry.hpp"

namespace CollisionDetectionAlgorithms {
  GLboolean intersection_between(const BoundingSphere * bs1, const BoundingSphere * bs2,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection );
  GLboolean intersection_between(const BoundingSphere * bs1, const BoundingAABB * bs2,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection );
  GLboolean intersection_between(const BoundingAABB * bs1, const BoundingAABB * bs2,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection );
  GLboolean intersection_between(const BoundingSphere * bs, const BoundingOBB * obb,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection );
  GLboolean intersection_between(const BoundingOBB * obb1, const BoundingOBB * obb2,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection );
  
  void closest_point_on_obb_to_point(const glm::vec3 & target_point, const BoundingOBB & obb,glm::vec3 & point_on_obb);
  GLboolean ray_triangle_intersection(const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec3 & v2,const glm::vec3 & p,const glm::vec3 & d);
}

#endif
