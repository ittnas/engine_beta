#ifndef RIGID_BODY_HPP
#define RIGID_BODY_HPP

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "object.hpp"
#include "movable.hpp"
#include "collision_object.hpp"

class RigidBody : virtual public CollisionObject, virtual public PhysicsObject {
private:
  //glm::vec3 center_of_mass;
  GLfloat inverse_mass; // Calculated from uniform density unless it is set directly
  GLfloat density; // uniform density, used to update mass.
  glm::mat3 inverse_moment_of_inertia;
  glm::vec3 angular_velocity;
  glm::vec3 velocity;
  glm::vec3 angular_velocity_delayed;
  glm::vec3 velocity_delayed;
  GLuint previous_tick;
  GLfloat coefficient_of_restitution;
  GLuint num_delayed_impulses;
  
  glm::vec3 calculate_center_of_mass(GLuint attribute_array_index = 0,GLuint density_array_index = -1); // From mesh.
  void calculate_moment_of_inertia(glm::mat3 & moment_of_inertia, glm::vec3 & center_of_mass,GLfloat & mass,GLuint position_array_index,GLuint density_array_index);
  static glm::mat3 outer(const glm::vec3 &,const glm::vec3 &);
  virtual void apply_force_effect(Force & force, GLfloat duration);
protected:

public:
  static void scale_mass(GLfloat & mass,GLfloat scale_factor);
  static void translate_moment_of_inertia(glm::mat3 & moment_of_inertia,const glm::vec3 & distance,GLfloat mass,const glm::vec3 & center_of_mass);
  static void rotate_moment_of_inertia(glm::mat3 & moment_of_inertia, const glm::mat3 & rotation_matrix);
  static void scale_moment_of_inertia(glm::mat3 & moment_inertia, GLfloat scale_factor);
  virtual void update_motion(GLuint tick);
  virtual void update_delayed_parameters();
  void apply_impulse(const glm::vec3 & impulse,const glm::vec3 & position);
  //void apply_impulse(const glm::vec3 & impulse);
  void apply_impulse_delayed(const glm::vec3 & impulse,const glm::vec3 & position);
  RigidBody();
  virtual ~RigidBody() {}
  void set_uniform_mass(GLfloat mass);
  void set_uniform_density(GLfloat density); // DOES NOT WORK ATM. DOES NOTHING.
  GLboolean set_density_attribute_array(GLuint attribute_array_index);
  GLboolean set_density_attribute_array(GLuint attribute_array_index, GLfloat total_mass);
  void move_mesh_to_center_of_mass(GLuint attribute_array_index = 0,GLuint density_array_index = -1);
  void set_moment_of_inertia_and_mass(GLuint position_array_index = 0,GLuint density_array_index = -1);
  void set_coefficient_of_restitution(GLfloat cor) {this->coefficient_of_restitution = cor;}
  GLfloat get_coefficient_of_restitution() const {return coefficient_of_restitution;}
  glm::vec3 get_velocity() const {return velocity;}
  glm::vec3 get_angular_velocity() const {return angular_velocity;}
  glm::vec3 get_velocity_delayed() const;
  GLfloat get_inverse_mass() const {return inverse_mass;}
  glm::mat3 get_inverse_moment_of_inertia() const {return inverse_moment_of_inertia;}
  virtual void collision_with(CollisionObject & co, const CollisionPair & cp);
  virtual void perform_collision_with(RigidBody * rb, const CollisionPair & cp);
  virtual void perform_collision_with(CollisionObject * rb, const CollisionPair & cp);
  
  // void apply_constraint(Constraint *, Movable *) //TODO
};

#endif
