#ifndef UNIFORM_MATERIAL_HPP
#define UNIFORM_MATERIAL_HPP

#include "object.hpp"
#include "uniform_material_buffer.hpp"
#include "movable.hpp"

class UniformMaterial : virtual public Object, virtual public Movable {
private:
  UniformMaterialBuffer * uniform_material_buffer;
public:
  UniformMaterial();
  virtual ~UniformMaterial() {}
  void set_ambient_color(glm::vec4 amc) {this->uniform_material_buffer->set_ambient_color(amc);}
  void set_specular_color(glm::vec4 amc) {this->uniform_material_buffer->set_specular_color(amc);}
  void set_diffusive_color(glm::vec4 amc) {this->uniform_material_buffer->set_diffusive_color(amc);}
  void set_shininess(GLfloat shininess) {this->uniform_material_buffer->set_shininess(shininess);}
 
  glm::vec4 get_ambient_color() const {return this->uniform_material_buffer->get_ambient_color();}
  glm::vec4 get_diffusive_color() const {return this->uniform_material_buffer->get_diffusive_color();}
  glm::vec4 get_specular_color() const {return this->uniform_material_buffer->get_specular_color();}
  GLfloat get_shininess() const {return this->uniform_material_buffer->get_shininess();}
};

#endif
