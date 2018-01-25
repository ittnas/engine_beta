#ifndef PHYSICS_MANAGER_HPP
#define PHYSICS_MANAGER_HPP

#include <vector>

#include "manager.hpp"
#include "physics_object.hpp"

class Force;

// Rename to physics engine?

class PhysicsManager : public Manager {
private:
  std::vector<PhysicsObject *> physics_objects;
  std::vector<Force *> forces;
public:
  PhysicsManager();
  virtual ~PhysicsManager() {}
  virtual void register_object(Object * object);
  virtual void deregister_object(Object * object);
  virtual void manage(); // This is performed right before drawing
  //void add_force_field(ForceField * force);
  void add_force(Force * force);
  void apply_forces(GLfloat duration);
  void update_motion(GLuint tick); // This is performed by action_on_worlds
};
#endif
