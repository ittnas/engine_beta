#include <algorithm>

#include "physics_manager.hpp"
#include "physics_object.hpp"

PhysicsManager::PhysicsManager() : Manager() {
  
}

void PhysicsManager::register_object(Object * object) {
  PhysicsObject * physics_object = dynamic_cast<PhysicsObject *>(object);
  if(physics_object != NULL) {
    physics_objects.push_back(physics_object);
  }
}

void PhysicsManager::deregister_object(Object * object) {
  PhysicsObject * physics_object = dynamic_cast<PhysicsObject *>(object);
  if(physics_object != NULL) {
    physics_objects.erase(std::remove(physics_objects.begin(), physics_objects.end(), physics_object), physics_objects.end());
    //physics_objects.erase(physics_object);
  }


}

void PhysicsManager::manage() {
  //TODO
}

void PhysicsManager::update_motion(GLuint tick) {
  for(auto child : physics_objects) {
    child->update_motion(tick);
  }
}

void PhysicsManager::add_force(Force * force) {
  forces.push_back(force);
}

void PhysicsManager::apply_forces(GLfloat duration) {
  for(auto force : forces) {
    for(auto object : physics_objects) {
      object->apply_force(*force,duration);
    }
  }
}
