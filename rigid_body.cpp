#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include "force.hpp"
#include "rigid_body.hpp"
#include "collision_response_algorithms.hpp"

/* DEPRACATED */

glm::vec3 RigidBody::calculate_center_of_mass(GLuint attribute_array_index,GLuint density_array_index) {
  glm::vec3 center_of_mass = glm::vec3(0,0,0);
  for(auto child : get_children()) {
    RigidBody * rb = dynamic_cast<RigidBody *>(child);
    if(rb != 0) {
      center_of_mass += rb->calculate_center_of_mass(attribute_array_index,density_array_index);
    }
  }
  if(get_shape() != 0) {
    Mesh * mesh = get_shape()->get_mesh();
    //center_of_mass += mesh->get_center_of_mass(attribute_array_index,density_array_index);
    glm::vec3 shape_center_of_mass;
    GLfloat shape_mass;
    mesh->get_center_of_mass(attribute_array_index,shape_center_of_mass,shape_mass);
    center_of_mass += shape_center_of_mass; // WTF, this is not correct.
  }
  assert(0); // THIS FUNCTION DOES NOT WORK CORRECTLY
  return center_of_mass;
}

void RigidBody::set_uniform_mass(GLfloat mass) {
  std::cout << "TODO Update inertia tensor. From rigid_body.cpp/set_uniform_mass" << std::endl;
  //TODO Update inertia tensor.
  this->inverse_mass = 1/mass;
}

void RigidBody::set_uniform_density(GLfloat density) {
  //TODO update inertia tensor, if it is already set.
  this->density = density;
}

/**
   Calculates the moment of inertia
 **/

void RigidBody::set_moment_of_inertia_and_mass(GLuint position_array_index,GLuint density_array_index) {
  glm::mat3 moment_of_inertia;
  glm::vec3 center_of_mass;
  GLfloat mass;
  calculate_moment_of_inertia(moment_of_inertia,center_of_mass,mass,position_array_index,density_array_index);
  // MOMENT OF INERTIA IS NEGATIVE!!
  this->inverse_moment_of_inertia = glm::inverse(moment_of_inertia);
  //this->center_of_mass = center_of_mass;
  set_center_of_mass(center_of_mass);
  // MASS HAS SOMETIMES INCORRECT SIGN!!
  this->inverse_mass = std::abs(1.0/(mass));
}

void RigidBody::scale_mass(GLfloat & mass, GLfloat scale_factor) {
  mass = std::pow(scale_factor,3)*mass;
}

// Change of plan. Calculate moment of inertia around the center of mass instead of zero. The moment of inertia needs to be rotated every time anyway, because the object is rotating.
/*
void RigidBody::calculate_moment_of_inertia(glm::mat3 & moment_of_inertia, glm::vec3 & center_of_mass,GLfloat & mass,GLuint position_array_index,GLuint density_array_index) {
  moment_of_inertia = glm::mat3(0.0);
  mass = 0.0;
  center_of_mass = glm::vec3(0.0);
  for(auto child : get_children()) {
    RigidBody * rb = dynamic_cast<RigidBody *>(child);
    if(rb != 0) {
      glm::mat3 target_moment_of_inertia = glm::mat3(0.0);
      glm::vec3 target_center_of_mass = glm::vec3(0,0,0);
      GLfloat target_mass = 0.0;
      // Get the moment of inertia of a child object with respect to point (0,0,0) of the current frame
      rb->calculate_moment_of_inertia(target_moment_of_inertia,target_center_of_mass,target_mass,position_array_index,density_array_index);
      //center_of_mass += target_mass*(target_center_of_mass + glm::vec3(rb->get_position()));
      center_of_mass = (center_of_mass*mass + target_center_of_mass*target_mass)/(mass + target_center_of_mass); // Addition of the centers of mass according to the formula r_cm = (r1*m1 + r2*m2)/(m1+m2)
      mass += target_mass; // mass is incremented only after the calculation of the center of mass
      moment_of_inertia += target_moment_of_inertia; // Momenta of inertia calculated with respect the the same reference point ((0,0,0) in this case) can be added.
    }
  }
  if(get_shape() != 0) {
    Mesh * mesh = get_shape()->get_mesh();
    glm::mat3 mesh_moment_of_inertia = mesh->get_moment_of_inertia(position_array_index);
    glm::vec3 mesh_center_of_mass = mesh->get_center_of_mass(position_array_index);
    GLfloat mesh_mass = mesh->get_volume(position_array_index); // This could be optimized. Volume is also calculated alongwith center of mass.
    // Next translate the reference point of the moment of inertia to be point (0,0,0) of this frame (now it is the center of mass of the mesh). No need to do any rotations or stuff. That comes later.
    //translate_moment_of_inertia(mesh_moment_of_inertia,glm::vec3(0,0,0),mesh_mass,mesh_center_of_mass);
    center_of_mass += (center_of_mass*mass + mesh_center_of_mass*mesh_mass)/(mass + mesh_mass);
    moment_of_inertia += mesh_moment_of_inertia;
    mass += mesh_mass;
    for(GLuint i = 0; i < 3; i++) {
      for(GLuint j = 0; j < 3; j++) {
	std::cout << moment_of_inertia[i][j] << ",\t";
      }
      std::cout << std::endl;
    }
  }
  // Next transform the mass and the center of mass to the center of the parent frame. Also remember to apply scale and rotations.
  scale_mass(mass,this->get_uniform_scale());
  scale_moment_of_inertia(moment_of_inertia,this->get_uniform_scale());
  rotate_moment_of_inertia(moment_of_inertia,glm::mat3_cast(this->get_orientation()));
  translate_moment_of_inertia(moment_of_inertia,glm::vec3(this->get_position()),mass,glm::vec3(0,0,0));
  std::cout << "From rigid_body.cpp/calculate_moment_of_inertia" << std::endl;
}
*/

/** 
    Calculates the moment of inertia around the center of mass of the object.
    Rules of moment of inertia:
    1. Additivity: I = I_1 + I_2, if the reference point if I_1 and I_2 are the same.
    2. Translation: ...
    3. Rotation: I_R = R I R^T
    4. Scaling: I_S = scale^5*I
 **/
void RigidBody::calculate_moment_of_inertia(glm::mat3 & moment_of_inertia, glm::vec3 & center_of_mass,GLfloat & mass,GLuint position_array_index,GLuint density_array_index) {
  moment_of_inertia = glm::mat3(0.0);
  mass = 0.0;
  center_of_mass = glm::vec3(0.0);
  for(auto child : get_children()) {
    RigidBody * rb = dynamic_cast<RigidBody *>(child);
    if(rb != 0) {
      glm::mat3 target_moment_of_inertia;
      glm::vec3 target_center_of_mass;
      GLfloat target_mass;
      GLfloat target_scale = rb->get_uniform_scale();
      glm::vec3 target_position = glm::vec3(rb->get_position());
      glm::fquat target_orientation = rb->get_orientation();
      // Get the moment of inertia of a child object. It is given with respect to the center of mass of the child.
      rb->calculate_moment_of_inertia(target_moment_of_inertia,target_center_of_mass,target_mass,position_array_index,density_array_index);
      
      //std::cout << "THE CORRECTIONS SHOULD BE CONTINUED FROM HERE. From rigid_body.cpp/calculate_moment_of_inertia." << std::endl;

      //center_of_mass = (center_of_mass*mass + target_center_of_mass*target_mass)/(mass + target_center_of_mass); // Addition of the centers of mass according to the formula r_cm = (r1*m1 + r2*m2)/(m1+m2)
      scale_mass(target_mass,target_scale);
      center_of_mass += target_mass*(target_center_of_mass + target_position);
      mass += target_mass;
      moment_of_inertia += target_moment_of_inertia; // Momenta of inertia calculated with respect the the same reference point ((0,0,0) in this case) can be added.
      scale_moment_of_inertia(target_moment_of_inertia,target_scale);
      rotate_moment_of_inertia(moment_of_inertia,glm::mat3_cast(target_orientation));
      translate_moment_of_inertia(moment_of_inertia,glm::vec3(0,0,0),target_mass,target_position);
    }
  }
  if(get_shape() != 0) {
    Mesh * mesh = get_shape()->get_mesh();
    glm::mat3 mesh_moment_of_inertia = mesh->get_moment_of_inertia(position_array_index);
    //glm::vec3 mesh_center_of_mass = mesh->get_center_of_mass(position_array_index);
    //GLfloat mesh_mass = mesh->get_volume(position_array_index); // This could be optimized. Volume is also calculated alongwith center of mass.
    glm::vec3 mesh_center_of_mass;
    GLfloat mesh_mass;
    mesh->get_center_of_mass(position_array_index,mesh_center_of_mass,mesh_mass);
    // Next translate the reference point of the moment of inertia to be point (0,0,0) of this frame (now it is the center of mass of the mesh). No need to do any rotations or stuff. That comes later.
    //translate_moment_of_inertia(mesh_moment_of_inertia,glm::vec3(0,0,0),mesh_mass,mesh_center_of_mass);
    center_of_mass += mesh_center_of_mass*mesh_mass;
    moment_of_inertia += mesh_moment_of_inertia;
    mass += mesh_mass;
    std::cout << "Mass: " << mass << std::endl;
    std::cout << "Center of mass: " << center_of_mass[0] << ", " << center_of_mass[1] << ", " << center_of_mass[2] << std::endl;
    std::cout << "Inertia tensor: " << std::endl;
    for(GLuint i = 0; i < 3; i++) {
      for(GLuint j = 0; j < 3; j++) {
	std::cout << moment_of_inertia[i][j] << ",\t";
      }
      std::cout << std::endl;
    }
  }
  std::cout << "From rigid_body.cpp/calculate_moment_of_inertia" << std::endl;
  // Next transform the mass and the center of mass to the center of the parent frame. Also remember to apply scale and rotations.
  /*
  scale_mass(mass,this->get_uniform_scale());
  scale_moment_of_inertia(moment_of_inertia,this->get_uniform_scale());
  rotate_moment_of_inertia(moment_of_inertia,glm::mat3_cast(this->get_orientation()));
  translate_moment_of_inertia(moment_of_inertia,glm::vec3(this->get_position()),mass,glm::vec3(0,0,0));
  */
  center_of_mass = center_of_mass/mass;
}


glm::mat3 RigidBody::outer(const glm::vec3 & vec1,const glm::vec3 & vec2) {
  GLfloat output[9] = {vec1.x*vec2.x,vec1.x*vec2.y,vec1.x*vec2.z,
		       vec1.y*vec2.x,vec1.y*vec2.y,vec1.y*vec2.z,
		       vec1.z*vec2.x,vec1.z*vec2.y,vec1.z*vec2.z};
  return glm::make_mat3(output);
}

/*
  Translates the moment of inertia from the center_of_mass to distance.
  mass - mass of the object
  distance - new reference point
  center_of_mass - old reference point
 */

void RigidBody::translate_moment_of_inertia(glm::mat3 & moment_of_inertia,const glm::vec3 & distance, GLfloat mass, const glm::vec3 & center_of_mass) {
  glm::mat3 deflection = mass*(outer(distance,distance) + outer(distance,center_of_mass) + outer(center_of_mass,distance));
  moment_of_inertia += glm::mat3(1.0)*(deflection[0][0] + deflection[1][1] + deflection[2][2]) - deflection;
}

void RigidBody::rotate_moment_of_inertia(glm::mat3 &moment_of_inertia, const glm::mat3 & rotation_matrix) {
  moment_of_inertia = rotation_matrix*moment_of_inertia*glm::transpose(rotation_matrix);
}

void RigidBody::scale_moment_of_inertia(glm::mat3 & moment_of_inertia,GLfloat scale_factor) {
  moment_of_inertia = GLfloat(std::pow(scale_factor,5))*moment_of_inertia;
}

RigidBody::RigidBody() : CollisionObject(), PhysicsObject(),density(1.0), previous_tick(-1), velocity(glm::vec3(0,0,0)), angular_velocity(glm::vec3(0,0,0)), coefficient_of_restitution(1.0), angular_velocity_delayed(glm::vec3(0,0,0)),velocity_delayed(glm::vec3(0,0,0)), num_delayed_impulses(0) {
  
}

void RigidBody::apply_impulse(const glm::vec3 & impulse, const glm::vec3 & position) {
  //glm::vec3 r = glm::vec3(this->get_position()) + get_center_of_mass();
  //glm::vec3 r = get_center_of_mass();
  glm::vec3 r = glm::vec3(this->get_position());
  glm::fquat orientation = this->get_orientation();
  GLfloat inverse_scale = 1.0/static_cast<GLfloat>(this->get_uniform_scale());
  GLfloat transformed_inverse_mass = this->get_inverse_mass();
  // The moment of inertia is calculated with respect to the zero of the model space, not the center of mass.
  //glm::vec3 r = glm::vec3(0,0,0);
  glm::mat3 transformed_inverse_moment_of_inertia = inverse_moment_of_inertia;
  // (RIR^T)^-1 = R I^-1 R^T
  scale_mass(transformed_inverse_mass,inverse_scale);
  scale_moment_of_inertia(transformed_inverse_moment_of_inertia,inverse_scale);
  rotate_moment_of_inertia(transformed_inverse_moment_of_inertia,glm::mat3_cast(orientation));
  glm::vec3 inertia_vector = glm::cross(position - r, impulse);
  angular_velocity += transformed_inverse_moment_of_inertia*inertia_vector;
  velocity += impulse*transformed_inverse_mass;
}

void RigidBody::apply_impulse_delayed(const glm::vec3 & impulse, const glm::vec3 & position) {
  //glm::vec3 r = glm::vec3(this->get_position()) + get_center_of_mass();
  //glm::vec3 r = get_center_of_mass();
  glm::vec3 r = glm::vec3(0,0,0);
  angular_velocity_delayed += inverse_moment_of_inertia*glm::cross(position - r, impulse);
  velocity_delayed += impulse*inverse_mass;
  num_delayed_impulses++;
}

void RigidBody::update_delayed_parameters() {
  if(num_delayed_impulses > 0) {
  angular_velocity += angular_velocity_delayed/static_cast<GLfloat>(num_delayed_impulses);
  angular_velocity_delayed = glm::vec3(0,0,0);
  velocity += velocity_delayed/static_cast<GLfloat>(num_delayed_impulses);
  velocity_delayed = glm::vec3(0,0,0);
  num_delayed_impulses = 0;
  }
}

glm::vec3 RigidBody::get_velocity_delayed() const {
  if(num_delayed_impulses == 0) {
    return glm::vec3(0,0,0);
  } else {
    return velocity_delayed/static_cast<GLfloat>(num_delayed_impulses);
  }
}

void RigidBody::update_motion(GLuint tick) {
  if(tick == -1) {
    previous_tick = tick;
    return;
  }
  GLfloat dt = (GLfloat)(tick-previous_tick)/1000;
  //std::cout << "dt: " << dt << std::endl;
  glm::vec4 new_position = get_position() + glm::vec4(velocity.x,velocity.y,velocity.z,0)*dt;
  set_position(new_position);
  GLfloat av_norm = std::sqrt(angular_velocity.x*angular_velocity.x+angular_velocity.y*angular_velocity.y+angular_velocity.z*angular_velocity.z);
  if(av_norm != 0)  {
    glm::fquat new_orientation = glm::fquat(std::cos(av_norm*dt/2),angular_velocity.x*std::sin(av_norm*dt/2)/av_norm,angular_velocity.y*std::sin(av_norm*dt/2)/av_norm,angular_velocity.z*std::sin(av_norm*dt/2)/av_norm);
    glm::fquat previous_orientation = get_orientation();
    new_orientation = new_orientation*previous_orientation;
    //std::cout << "Orientation: " << "x: " << new_orientation.x << " y: " << new_orientation.y << " z: " << new_orientation.z << " w: " << new_orientation.w << std::endl;
  set_orientation(new_orientation);
  }
  previous_tick = tick;
}

void RigidBody::apply_force_effect(Force & force, GLfloat duration) {
  glm::vec3 force_vector;
  switch(force.get_type()) {
  case GRAVITY:
    force_vector = 1.0f/this->get_inverse_mass()*force.get_strength(glm::vec3(get_position()) + get_center_of_mass());
    break;
  case ELECTROMAGNETIC:
    force_vector = glm::vec3(0,0,0);
    break;
  case LORENZ:
    force_vector = glm::vec3(0,0,0);
    break;
  }
  apply_impulse(force_vector*duration,glm::vec3(get_position()) + get_center_of_mass());
}

void RigidBody::collision_with(CollisionObject & co, const CollisionPair & cp) {
  co.perform_collision_with(this,cp);
}

void RigidBody::perform_collision_with(RigidBody * rb, const CollisionPair & cp) {
  CollisionResponseAlgorithms::collision_between(rb,this,cp);
}

void RigidBody::perform_collision_with(CollisionObject * co, const CollisionPair & cp) {
  CollisionResponseAlgorithms::collision_between(co,this,cp);
}
