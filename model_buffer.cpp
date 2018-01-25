#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include "model_buffer.hpp"


//MatrixStack * ModelBuffer::model_stack = new MatrixStack();
//MatrixStack * ModelBuffer::model_stack = GlobalContext::model_stack;

ModelBuffer::ModelBuffer() : Buffer("model_buffer",buffer_length), position(glm::vec4(0,0,0,1)),orientation(glm::fquat(1,0,0,0)), scale(glm::vec3(1,1,1)), center_of_mass(glm::vec3(0,0,0)) {
  model_stack = GlobalContext::model_stack;
}

void ModelBuffer::update_buffer_phase1(Object * parent) {
  //Update the buffer
}

void ModelBuffer::update_model_matrix() {
  model_stack->translate(glm::vec4(position.x+center_of_mass.x,position.y+center_of_mass.y,position.z+center_of_mass.z,position.w));
  model_stack->rotate(orientation);
  model_stack->scale(scale);
  model_stack->translate(-center_of_mass);
  model_matrix = model_stack->top();
}

void ModelBuffer::update_buffer_phase2(Object * parent) {
  glBindBuffer(GL_UNIFORM_BUFFER,get_ubo_pointer());
  glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::mat4),glm::value_ptr(model_matrix));
  glm::mat3 normal_trans_matrix(model_matrix);
  normal_trans_matrix = glm::transpose(glm::inverse(normal_trans_matrix)); //Effeciency could be possibly improved, if a normal_trans_matrix would be a variable, and it would be updated in update_model_matrix() function.

  // This ensures that the data is loaded in a correct format to the shader (mat3 is quite tricky in std140)
  GLfloat normal_data[48]; // 3*4*sizeof(GLfloat)
  memcpy(normal_data, &normal_trans_matrix[0][0],3*sizeof(GLfloat));
  memcpy(normal_data + 4, &normal_trans_matrix[1][0],3*sizeof(GLfloat));
  memcpy(normal_data + 8, &normal_trans_matrix[2][0],3*sizeof(GLfloat));
  glBufferSubData(GL_UNIFORM_BUFFER,sizeof(glm::mat4),48,normal_data);
  
  glBindBuffer(GL_UNIFORM_BUFFER,0);
}

void ModelBuffer::pre_update_function() {
  model_stack->push();
  update_model_matrix();
}

void ModelBuffer::post_update_function() {
  model_stack->pop();
}

void ModelBuffer::pre_draw_function() {

}

void ModelBuffer::post_draw_function() {

}
/*
void ModelBuffer::pre_update_function() {

}

void ModelBuffer::post_update_function() {
  
}
*/
/*
GLuint ModelBuffer::get_buffer_length() {
  return 0;
}
*/

GLfloat ModelBuffer::get_uniform_scale() {
  if(scale.x != scale.y || scale.y != scale.z) {
    throw 1;
  }
  return scale.x;
}

void ModelBuffer::set_center_of_mass(const glm::vec3 & com) {
  this->center_of_mass = com;
}

glm::vec3 ModelBuffer::get_center_of_mass() const {
  return this->center_of_mass;
}
