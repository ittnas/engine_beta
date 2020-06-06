#include "view_buffer.hpp"
#include "movable.hpp"
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

ViewBuffer::ViewBuffer() : Buffer("view_buffer",buffer_length), forward_dir(glm::vec3(0,0,-1)),up_dir(glm::vec3(0,1,0)), forward(glm::fquat(1,0,0,0)), up(glm::fquat(1,0,0,0)) {
  
}

glm::mat4 ViewBuffer::construct_view_matrix(glm::vec4 position) {
  glm::vec3 up_vec = glm::mat3_cast(up)*up_dir;
  up_vec = glm::normalize(up_vec);
  glm::vec3 forward_vec = glm::mat3_cast(forward)*forward_dir;
  glm::vec3 side = glm::cross(forward_vec,up_vec);
  side = glm::normalize(side);
  up_vec = glm::cross(side,forward_vec);
  
  /*glm::mat4 look_at_mat(side.x,side.y,side.z,0,
			up_vec.x,up_vec.y,up_vec.z,0,
			-forward_vec.x,-forward_vec.y,-forward_vec.z,0,
			-position.x,-position.y,-position.z,1);*/

glm::mat4 look_at_mat(side.x,side.y,side.z,0,
			up_vec.x,up_vec.y,up_vec.z,0,
			-forward_vec.x,-forward_vec.y,-forward_vec.z,0,
		       0,0,0,1);
 glm::mat4 trans_mat(1.0);
 trans_mat[3] = glm::vec4(-position.x,-position.y,-position.z,1.0);
 view_matrix = look_at_mat*trans_mat;
 return view_matrix;
}

void ViewBuffer::update_view_matrix(glm::vec4 position) {
  this->view_matrix = construct_view_matrix(position);
}


void ViewBuffer::update_buffer(glm::vec4 position) {
  this->view_matrix = construct_view_matrix(position);
  glBindBuffer(GL_UNIFORM_BUFFER,get_ubo_pointer());
  glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::mat4),glm::value_ptr(view_matrix));
  glBindBuffer(GL_UNIFORM_BUFFER,0);
}


void ViewBuffer::update_buffer_phase1(Object * parent) {
  //if(active) {
    Movable * mv = dynamic_cast<Movable *>(parent);
    if(mv == NULL) {
      //Cast failed
      assert(0);
    }
    glm::vec4 pos = mv->get_position();
    this->view_matrix = construct_view_matrix(pos);
    glBindBuffer(GL_UNIFORM_BUFFER,get_ubo_pointer());
    glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::mat4),glm::value_ptr(view_matrix));
    glBindBuffer(GL_UNIFORM_BUFFER,0);
    //}
}

glm::mat4 ViewBuffer::get_view_matrix() const {
  return view_matrix;
}
