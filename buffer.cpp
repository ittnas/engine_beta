#include <assert.h>

#include "buffer.hpp"
#include "ubo.hpp"
#include "buffer_list.hpp"
#include "gl_constants.hpp"

/**
   Creates the UBO when called for the first time. This ensures, that the UBO is allocated in the GPU only once.
THIS FUNCTIONALITY IS NOW MOVED TO buffer_list.hpp.
 **/

Buffer::Buffer(std::string ubo_name,GLuint buffer_length,GLint buffer_update_frequency) {
  //TODO Modify to match moving ubo functionality to buffer_list
  /*
  std::unordered_map<std::string,UBO*>::iterator it = initialized_buffers.find(ubo_name);
  if(it==initialized_buffers.end()) {
    // Buffer is not initialized
    UBO* ubo =  init_ubo(ubo_name,buffer_length,buffer_update_frequency);
    initialized_buffers.insert(std::pair<std::string,UBO *>(ubo_name,ubo)); // Buffer is now created.
    this->ubo = ubo;
  } else { // else buffer is already initialized, and nothing needs to be done.
    this->ubo = it->second;
  }
  */
  this->ubo = BufferList::init_buffer(ubo_name,buffer_length,buffer_update_frequency);
}
/*
MOVED TO buffer_list.hpp

UBO * Buffer::init_ubo(std::string ubo_name, GLuint buffer_length,GLint buffer_update_frequency) {
  GLuint ubo_pointer = create_buffer(buffer_length,buffer_update_frequency);
  GLuint binding_point = generate_unique_binding_point();
  glBindBufferRange(GL_UNIFORM_BUFFER,binding_point,ubo_pointer,0,buffer_length);
  UBO * ubo = new UBO(ubo_name,binding_point,ubo_pointer);
  return ubo;
}
*/
/*
MOVED TO buffer_list.cpp
GLuint Buffer::generate_unique_binding_point() {
  GLuint new_binding_point = unique_binding_point_counter;
  unique_binding_point_counter++;
  assert(new_binding_point < MAX_NBR_BINDING_POINTS);
  return new_binding_point;
}
*/
/**
Creates a new uniform buffer object (UBO) with size buffer_size (in bytes) and update frequency hint buffer_update_frequency. OpenGL pointer to the UBO is returned.
MOVED to buffer_list.cpp
 **/
/*
GLuint Buffer::create_buffer(GLuint buffer_size, GLint buffer_update_frequency) {
  GLuint ubo_pointer;
  GLubyte * empty = new GLubyte[buffer_size];
  for(GLuint i = 0; i < buffer_size; i++) {
    empty[i] = 0;
  }
  glGenBuffers(1,&ubo_pointer);
  glBindBuffer(GL_UNIFORM_BUFFER,ubo_pointer);
  glBufferData(GL_UNIFORM_BUFFER,buffer_size,empty,buffer_update_frequency);
  glBindBuffer(GL_UNIFORM_BUFFER,0);
  delete[] empty;
  return ubo_pointer;
}
*/
std::string Buffer::get_ubo_name() {
  return ubo->get_ubo_name();
}

GLuint Buffer::get_binding_point() {
  return ubo->get_binding_point();
}

GLuint Buffer::get_ubo_pointer() {
  return ubo->get_ubo_pointer();
}

