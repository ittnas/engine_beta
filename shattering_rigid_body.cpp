#include "shattering_rigid_body.hpp"
#include "collision_response_algorithms.hpp"

ShatteringRigidBody::ShatteringRigidBody() : RigidBody() {

}

void ShatteringRigidBody::collision_with(CollisionObject & co, const CollisionPair & cp) {
  co.perform_collision_with(this,cp);
}

void ShatteringRigidBody::perform_collision_with(RigidBody * rb, const CollisionPair & cp) {
  CollisionResponseAlgorithms::collision_between(rb,this,cp);
}
