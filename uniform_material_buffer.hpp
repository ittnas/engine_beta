#ifndef UNIFORM_MATERIAL_BUFFER_HPP
#define UNIFORM_MATERIAL_BUFFER_HPP

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "buffer.hpp"

class UniformMaterialBuffer : public Buffer {
private:
  static const GLuint buffer_length = (3*4+1)*sizeof(GLfloat); //Specular, diffusive, ambient, shininess
  glm::vec4 ambient_color;
  glm::vec4 diffusive_color;
  glm::vec4 specular_color;
  GLfloat shininess;
public:
  UniformMaterialBuffer();
  virtual ~UniformMaterialBuffer() {}
  virtual void update_buffer_phase2(Object * parent);
   
  //Setters
  void set_ambient_color(glm::vec4 amc) {this->ambient_color = amc;}
  void set_diffusive_color(glm::vec4 dic) {this->diffusive_color = dic;}
  void set_specular_color(glm::vec4 spc) {this->specular_color = spc;}
  void set_shininess(GLfloat shininess) {this->shininess = shininess;}
  
  //Getters
  glm::vec4 get_ambient_color() const {return this->ambient_color;}
  glm::vec4 get_diffusive_color() const {return this->diffusive_color;}
  glm::vec4 get_specular_color() const {return this->specular_color;}
  GLfloat get_shininess() const {return this->shininess;}
};

#endif
