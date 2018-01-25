#define GLM_FORCE_RADIANS
#include <glm/gtx/norm.hpp>

#include "force.hpp"

Force::Force(ForceType type) : force_type(type) {

} 

glm::vec3 Force::get_strength(const glm::vec3 & position) const {
  return get_strength_sub_class(position);
}

ConstantForce::ConstantForce(ForceType type, glm::vec3 force_vector) : Force(type),force_vector(force_vector) {
  
}

glm::vec3 ConstantForce::get_strength_sub_class(const glm::vec3 & position) const {
  return force_vector;
}

PointForce::PointForce(ForceType type,glm::vec3 force_position,GLfloat field_strength,GLfloat max_strength) : Force(type), force_position(force_position), field_strength(field_strength),max_strength(max_strength) {
  
}

glm::vec3 PointForce::get_strength_sub_class(const glm::vec3 & position) const {
  return glm::normalize(force_position-position)*field_strength/glm::distance2(position,force_position);
}


CenterOfMassForce::CenterOfMassForce(ForceType type, const Movable * object,GLfloat field_strength) : Force(type), object(object),field_strength(field_strength) {
  
}

glm::vec3 CenterOfMassForce::get_strength_sub_class(const glm::vec3 & position) const {
  glm::vec3 com = object->get_center_of_mass() + glm::vec3(object->get_position());
  GLfloat distance2 = glm::distance2(position,com);
  if(distance2 > 0.00001) {
    return glm::normalize(com-position)*field_strength/glm::distance2(position,com);
  } else {
    return glm::vec3(0,0,0);
  }
}
