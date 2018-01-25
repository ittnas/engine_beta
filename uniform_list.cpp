#include <stdexcept>

#include "uniform_list.hpp"

GLuint UniformList::current_program_ident = 0;
std::unordered_map<std::string,std::unordered_map<GLuint,GLint>> UniformList::uniforms;

void UniformList::add_uniform(std::string uniform_name,GLuint program_ident,GLint uniform_location) {
  uniforms[uniform_name][program_ident] = uniform_location;
}

GLint UniformList::get_uniform_location(std::string uniform_name) {
  try {
    return uniforms.at(uniform_name).at(current_program_ident);
  } catch (const std::out_of_range& oor) {
    return -1;
  }
}
