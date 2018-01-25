#ifndef PHYSICS_ENGINE_HPP
#define PHYSICS_ENGINE_HPP

#include "physics_object.hpp"

// NOT USED
class PhysiscEngine {
public:
  PhysiscEngine() {}
  virtual ~PhysiscEngine() {}
  void add_object(PhysicsObject * po);
  
};

#endif
