#ifndef COLLISION_CALLBACK_FUNCTION_HPP
#define COLLISION_CALLBACK_FUNCTION_HPP

#include "collision_pair.hpp"
//#include "uniform_color_collision_object.hpp"

class UniformColorCollisionObject;

class CollisionCallbackFunction {
public:
  CollisionCallbackFunction() {};
  virtual ~CollisionCallbackFunction() {}
  virtual void apply(const CollisionPair &) = 0;
};

class MaterialColorCollisionCallbackFunction : public CollisionCallbackFunction {
private:
  UniformColorCollisionObject * ucco;
public:
  MaterialColorCollisionCallbackFunction() {}
  virtual ~MaterialColorCollisionCallbackFunction() {}
  void apply(const CollisionPair &);
  void register_target(UniformColorCollisionObject * ucco) {this->ucco = ucco;}
};

#endif
