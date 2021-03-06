#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <string>
#include <unordered_map>
#include <GL/glew.h>

#include "ubo.hpp"

/**
Buffer class forms the interface between the UBOs in the GPU and the data of the Objects in the CPU. It also provides means to transfer the data from the CPU to the GPU. Buffers provide means to update data in the GPU in two phases:
PHASE I: The buffer is guaranteed to be updated before any PHASE II updates.
PHASE II: This updates are made after PHASE I updates.

When the Buffer of certain type is called for the very first time, it initializes the UBO, and creates a binding point. That binding point can be used to link programs to the UBO.

 **/

class Object;

class Buffer {
  //Variables
private:
  //static GLuint unique_binding_point_counter;
  //static std::unordered_map<std::string,UBO *> initialized_buffers;
  UBO * ubo;
protected:
  GLuint binding_point;
  std::string ubo_name;
  //Functions
private:
  //UBO* init_ubo(std::string ubo_name,GLuint buffer_length,GLint buffer_update_frequency);
  //static GLuint generate_unique_binding_point();
protected:
  Buffer(std::string ubo_name,GLuint buffer_length,GLint buffer_update_frequency = GL_STREAM_DRAW); // Creates buffer and binds it to an CPU abstraction of openGL buffer.
public :
  virtual void update_buffer_phase2(Object * parent) {} // Fetch the required data from parent.
  virtual void update_buffer_phase1(Object * parent) {} // Fetch the required data from parent
  virtual void update_uniforms(Object * parent) {}
  //virtual GLuint get_buffer_length() = 0;
  //  GLuint create_buffer(GLuint buffer_length, GLint buffer_update_frequency);
  virtual ~Buffer() {}
  std::string get_ubo_name();
  GLuint get_binding_point();
  GLuint get_ubo_pointer();
  virtual void pre_draw_function() {}
  virtual void post_draw_function() {}
  virtual void pre_update_function() {}
  virtual void post_update_function() {}
};

#endif
