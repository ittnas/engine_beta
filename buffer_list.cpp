#include <stdexcept>
#include <assert.h>

#include "buffer_list.hpp"
#include "gl_constants.hpp"


GLuint BufferList::unique_binding_point_counter = 0;
std::unordered_map<std::string,UBO *> BufferList::initialized_buffers;

UBO * BufferList::init_buffer(std::string ubo_name, GLuint buffer_length,GLint buffer_update_frequency) {
  try {
    UBO * old_ubo = initialized_buffers.at(ubo_name);
    if(!(old_ubo->is_buffer_created())) {
      // Binding point allocated, buffer not created. Create it now.
      GLuint ubo_pointer = create_buffer(buffer_length,buffer_update_frequency);
      old_ubo->add_ubo_pointer(ubo_pointer);
      glBindBufferRange(GL_UNIFORM_BUFFER,old_ubo->get_binding_point(),ubo_pointer,0,buffer_length);
    }
    return old_ubo;
  } catch (const std::out_of_range& oor) {
    GLuint binding_point = generate_unique_binding_point();
    GLuint ubo_pointer = create_buffer(buffer_length,buffer_update_frequency); 
    UBO * new_ubo = new UBO(ubo_name,binding_point,ubo_pointer);
    initialized_buffers[ubo_name] = new_ubo;
    glBindBufferRange(GL_UNIFORM_BUFFER,binding_point,ubo_pointer,0,buffer_length);
    return new_ubo;
  }
}

GLuint BufferList::generate_unique_binding_point() {
  GLuint new_binding_point = unique_binding_point_counter;
  unique_binding_point_counter++;
  assert(new_binding_point < MAX_NBR_BINDING_POINTS);
  return new_binding_point;
}

GLuint BufferList::get_binding_point(std::string ubo_name) {
  GLuint binding_point;
  try {
    binding_point = initialized_buffers.at(ubo_name)->get_binding_point();
  } catch (const std::out_of_range& oor) {
    binding_point = generate_unique_binding_point();
    UBO * new_ubo = new UBO(ubo_name,binding_point); // Buffer is not created, but it already has a binding point waiting.
    initialized_buffers[ubo_name] = new_ubo;
  }
  return binding_point;
}

/**
Creates a new uniform buffer object (UBO) with size buffer_size (in bytes) and update frequency hint buffer_update_frequency. OpenGL pointer to the UBO is returned.
 **/

GLuint BufferList::create_buffer(GLuint buffer_size, GLint buffer_update_frequency) {
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
