#include "uniform_color_collision_object.hpp"

UniformColorCollisionObject::UniformColorCollisionObject() : CollisionObject(), UniformMaterial() {

}

void UniformColorCollisionObject::add_collision_callback_function_and_register(std::shared_ptr<MaterialColorCollisionCallbackFunction> mcccf) {
  mcccf->register_target(this);
  this->add_collision_callback_function(mcccf);
}
