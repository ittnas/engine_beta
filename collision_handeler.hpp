#ifndef COLLISION_HANDLER_HPP
#define COLLISION_HANDLER_HPP

#include "object.hpp"
class CollisionHandeler {
public:
  virtual GLboolean accept_object(Object * object) = 0;
  virtual void handle_collision(Object * object);
};

#endif
