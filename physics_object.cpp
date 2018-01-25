#include "physics_object.hpp"

void PhysicsObject::apply_force(Force & force, GLfloat duration) {
  if(accepted_forces[force.get_type()]) {
    apply_force_effect(force,duration);
  }
}
