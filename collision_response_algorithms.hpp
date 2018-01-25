#ifndef COLLISION_RESPONSE_ALGORITHMS_HPP
#define COLLISION_RESPONSE_ALGORITHMS_HPP

#include "rigid_body.hpp"
#include "shattering_rigid_body.hpp"

namespace CollisionResponseAlgorithms {
  void collision_between(CollisionObject *, RigidBody *,const CollisionPair & cp);
  void collision_between(CollisionObject *, CollisionObject *,const CollisionPair & cp);
  void collision_between(RigidBody *, RigidBody *, const CollisionPair & cp);
  void collision_between(RigidBody *, ShatteringRigidBody *, const CollisionPair & cp);
}

#endif
