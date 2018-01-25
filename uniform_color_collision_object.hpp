#ifndef UNIFORM_COLOR_COLLISION_OBJECT_HPP
#define UNIFORM_COLOR_COLLISION_OBJECT_HPP

#include <memory>

#include "collision_object.hpp"
#include "rigid_body.hpp"
#include "uniform_material.hpp"
#include "collision_callback_function.hpp"

class UniformColorCollisionObject : virtual public RigidBody, virtual public UniformMaterial {
public:
  UniformColorCollisionObject();
  virtual ~UniformColorCollisionObject() {}
  void add_collision_callback_function_and_register(std::shared_ptr<MaterialColorCollisionCallbackFunction>);
};
#endif
