#ifndef PHYSICS_OBJECT_HPP
#define PHYSICS_OBJECT_HPP

#include <map>

#include "object.hpp"
#include "force.hpp"

// Add movable to list of inherited objects.
class PhysicsObject : virtual public Object {
private:
std::map<ForceType,GLboolean> accepted_forces;
virtual void apply_force_effect(Force & force, GLfloat duration) = 0;
public:
PhysicsObject() : Object() {}
virtual ~PhysicsObject() {}
virtual void update_motion(GLuint tick) = 0;
void apply_force(Force & force, GLfloat duration);
void toggle_force(ForceType force_type,GLboolean onoff) {accepted_forces[force_type] = onoff;}
};
#endif
