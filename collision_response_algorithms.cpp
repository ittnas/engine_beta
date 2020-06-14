#include <GL/glew.h>
#include "collision_response_algorithms.hpp"

void CollisionResponseAlgorithms::collision_between(CollisionObject *, CollisionObject *,const CollisionPair & cp) {
  //std::cout << "co vs co" << " (from collision_response_algorithms.cpp)"<< std::endl;
  // Nothing to be done.
}

void CollisionResponseAlgorithms::collision_between(CollisionObject *, RigidBody *, const CollisionPair & cp) {
  //std::cout << "co vs rb" << " (from collision_response_algorithms.cpp)"<< std::endl;
  // Nothing here either.
}

void CollisionResponseAlgorithms::collision_between(RigidBody * rb1, RigidBody * rb2, const CollisionPair & cp) {
  glm::vec3 n = cp.get_normal_of_intersection();
  /*
  if(glm::dot(n,vr) > 0) { // THIS IS A TEMPORARY FIX FOR AABB, which collide again after they start rotatiing.
    return;
  }
  */
  GLfloat cor1 = rb1->get_coefficient_of_restitution();
  GLfloat cor2 = rb2->get_coefficient_of_restitution();
  GLfloat e = cor1*cor2;
  glm::vec3 center_of_mass1 = glm::vec3(rb1->get_position()) + rb1->get_center_of_mass();
  glm::vec3 center_of_mass2 = glm::vec3(rb2->get_position()) + rb2->get_center_of_mass();
  glm::vec3 point_of_intersection = cp.get_point_of_intersection();
  glm::vec3 r1 = cp.get_point_of_intersection() - center_of_mass1;
  glm::vec3 rhat1 = glm::normalize(r1);
  glm::vec3 r2 = cp.get_point_of_intersection() - center_of_mass2;
  glm::vec3 rhat2 = glm::normalize(r2);
  
  glm::vec3 v1 = rb1->get_velocity() + glm::cross(rb1->get_angular_velocity(),r1);
  glm::vec3 v2 = rb2->get_velocity() + glm::cross(rb2->get_angular_velocity(),r2);
  //glm::vec3 vr = rb1->get_velocity() - rb2->get_velocity();
  glm::vec3 vr = v1 - v2;
  GLfloat upper = glm::dot(vr,n);
  //GLfloat dot_debug1 = glm::dot(n,r2);
  //GLfloat dot_debug2 = glm::dot(n,r1);
  glm::mat3 transformed_inverse_moment_of_inertia1 = rb1->get_inverse_moment_of_inertia();
  glm::mat3 transformed_inverse_moment_of_inertia2 = rb2->get_inverse_moment_of_inertia();
  GLfloat transformed_inverse_mass1 = rb1->get_inverse_mass();
  GLfloat transformed_inverse_mass2 = rb2->get_inverse_mass();
  GLfloat inverse_scale1 = 1.0f/rb1->get_uniform_scale();
  GLfloat inverse_scale2 = 1.0f/rb2->get_uniform_scale();
  glm::mat3 orientation1 = glm::mat3_cast(rb1->get_orientation());
  glm::mat3 orientation2 = glm::mat3_cast(rb2->get_orientation());
  RigidBody::scale_mass(transformed_inverse_mass1,inverse_scale1);
  RigidBody::scale_mass(transformed_inverse_mass2,inverse_scale2);
  
  RigidBody::scale_moment_of_inertia(transformed_inverse_moment_of_inertia1,inverse_scale1);
  RigidBody::scale_moment_of_inertia(transformed_inverse_moment_of_inertia2,inverse_scale2);
  RigidBody::rotate_moment_of_inertia(transformed_inverse_moment_of_inertia1,orientation1);
  RigidBody::rotate_moment_of_inertia(transformed_inverse_moment_of_inertia2,orientation2);
  
  //GLfloat mass_term1 = transformed_inverse_mass1*std::pow(glm::dot(n,rhat1),2);
  GLfloat mass_term1 = transformed_inverse_mass1;
  //GLfloat mass_term2 = transformed_inverse_mass2*std::pow(glm::dot(n,rhat2),2);
  GLfloat mass_term2 = transformed_inverse_mass2;
  GLfloat inertia = glm::dot(transformed_inverse_moment_of_inertia1*(glm::cross(glm::cross(r1,n),r1)) + transformed_inverse_moment_of_inertia2*(glm::cross(glm::cross(r2,n),r2)),n);
  GLfloat j = -(1 + e)*upper/(mass_term1 + mass_term2 + inertia);

  glm::vec3 velocity_before1 = rb1->get_velocity() + rb1->get_velocity_delayed();
  glm::vec3 velocity_before2 = rb2->get_velocity() + rb2->get_velocity_delayed();
  
  //rb1->apply_impulse_delayed(+j*n,cp.get_point_of_intersection());
  //rb2->apply_impulse_delayed(-j*n,cp.get_point_of_intersection());
  rb1->apply_impulse(+j*n,cp.get_point_of_intersection());
  rb2->apply_impulse(-j*n,cp.get_point_of_intersection());

  GLfloat mass1 = 1.0f/mass_term1;
  GLfloat mass2 = 1.0f/mass_term2;
  GLfloat mass_sum = mass1 + mass2;
  rb1->set_position(rb1->get_position() + glm::vec4(mass2/mass_sum*n*cp.get_depth_of_intersection()/GLfloat(2.0),0.0));
  rb2->set_position(rb2->get_position() - glm::vec4(mass1/mass_sum*n*cp.get_depth_of_intersection()/GLfloat(2.0),0.0));

  //rb1->apply_impulse(glm::vec3(-30000,0,0),rb1->get_center_of_mass());
  //rb2->apply_impulse(glm::vec3(30000,0,0),rb2->get_center_of_mass());
  glm::vec3 velocity_after1 = rb1->get_velocity() + rb1->get_velocity_delayed();
  glm::vec3 velocity_after2 = rb2->get_velocity() + rb2->get_velocity_delayed();
  //std::cout << "rb vs rb" << " v1 before: " << glm::length(velocity_before1) << " v1 after: " << glm::length(velocity_after1) << "| v2 before " << glm::length(velocity_before2) << " v2 after " << glm::length(velocity_after2) <<  " (from collision_response_algorithms.cpp)"<< std::endl;
}

void CollisionResponseAlgorithms::collision_between(RigidBody * rb, ShatteringRigidBody * srb, const CollisionPair & cp) {
  
}
