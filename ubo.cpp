#include <string>

#include "ubo.hpp"

UBO::UBO(std::string ubo_name, GLuint binding_point, GLuint ubo_pointer) : ubo_name(ubo_name),binding_point(binding_point),ubo_pointer(ubo_pointer), buffer_created(GL_TRUE) {

}

UBO::UBO(std::string ubo_name, GLuint binding_point) : ubo_name(ubo_name),binding_point(binding_point),ubo_pointer(-1),buffer_created(GL_FALSE) {
  
}

void UBO::add_ubo_pointer(GLuint pointer) {
  this->ubo_pointer = pointer;
  this->buffer_created = GL_TRUE;
}
