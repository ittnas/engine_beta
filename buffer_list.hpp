#ifndef BUFFER_LIST_HPP
#define BUFFER_LIST_HPP

#include <GL/glew.h>
#include <unordered_map>
#include <string>

#include "ubo.hpp"

class BufferList {
private:
  static GLuint unique_binding_point_counter;
  static std::unordered_map<std::string,UBO *> initialized_buffers;
  static GLuint generate_unique_binding_point();
  static GLuint create_buffer(GLuint buffer_length, GLint buffer_update_frequency);

public:
  static UBO* init_buffer(std::string ubo_name,GLuint buffer_length,GLint buffer_update_frequency); // Buffers should call this
  static GLuint get_binding_point(std::string ubo_name); // Programs should call this
};

#endif
