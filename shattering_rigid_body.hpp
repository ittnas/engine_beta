#ifndef SHATTERING_RIGID_BODY_HPP
#define SHATTERING_RIGID_BODY_HPP

#include "rigid_body.hpp"

class ShatteringRigidBody : virtual public RigidBody {
public:
  ShatteringRigidBody();
  virtual ~ShatteringRigidBody() {}
  virtual void collision_with(CollisionObject & co, const CollisionPair & cp);
  virtual void perform_collision_with(RigidBody * rb, const CollisionPair & cp);
  //virtual void perform_collision_with(CollisionObject * rb, const CollisionPair & cp); // Use the version of RigidBody
  //virtual void perform_collision_with(ShatteringRigidBody * rb, const CollisionPair & cp); // Use the version of RigidBody
};
#endif
