#ifndef UNIFORM_LIST_HPP
#define UNIFORM_LIST_HPP

#include <string>
#include <unordered_map>
#include <GL/glew.h>

class UniformList {
private:
  static GLuint current_program_ident;
  //static std::unordered_map<std::string¸std::unordered_map<GLuint,GLint>> uniforms; // <Name of the uniform,<program ident,uniformLocation>>
  static std::unordered_map<std::string, std::unordered_map<GLuint,GLint>> uniforms;
public:
  static void set_current_program_ident(GLuint cpi) {current_program_ident = cpi;}
  static void add_uniform(std::string uniform_name,GLuint program_ident,GLint uniform_location);
  static GLint get_uniform_location(std::string uniform_name);
};
#endif
