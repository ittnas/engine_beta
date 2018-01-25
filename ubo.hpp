#ifndef UBO_HPP
#define UBO_HPP

#include<GL/glew.h>
#include <string>
class UBO {

private:
  std::string ubo_name;
  GLuint binding_point;
  GLuint ubo_pointer;
  GLboolean buffer_created;
public:
  UBO(std::string ubo_name,GLuint binding_point, GLuint ubo_pointer);
  UBO(std::string ubo_name,GLuint binding_point);
  std::string get_ubo_name() {return ubo_name;}
  GLuint get_binding_point() {return binding_point;}
  GLuint get_ubo_pointer() {return ubo_pointer;}
  GLboolean is_buffer_created() {return this->buffer_created;}
  void add_ubo_pointer(GLuint pointer);
};

#endif
