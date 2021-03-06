#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "lightning_buffer.hpp"
#include "object.hpp"
#include "movable.hpp"

class Light : virtual public Object, virtual public Movable {
private:
  LightningBuffer * lightning_buffer;
public:
  Light();
  glm::vec4 get_intensity() const {return lightning_buffer->get_intensity();}
  GLfloat get_exponent() const {return lightning_buffer->get_exponent();}
  GLfloat get_cutoff() const {return lightning_buffer->get_cutoff();}
  GLfloat get_kc() const {return lightning_buffer->get_kc();}
  GLfloat get_kl() const {return lightning_buffer->get_kl();}
  GLfloat get_kq() const {return lightning_buffer->get_kq();}

  void set_intensity(glm::vec4 intensity) {
    lightning_buffer->set_intensity(intensity);
  }
  void set_exponent(GLfloat exponent) {
    lightning_buffer->set_exponent( exponent);
  }
  void set_cutoff(GLfloat cutoff) {
    lightning_buffer->set_cutoff(cutoff);
  }
  void set_kc(GLfloat kc) {
    lightning_buffer->set_kc(kc);
  }
  void set_kl(GLfloat kl) {
    lightning_buffer->set_kl(kl);
  }
  void set_kq(GLfloat kq) {
    lightning_buffer->set_kq(kq);
  }
  void set_light_index(GLuint index) {
    this->lightning_buffer->set_light_index(index);
  }
  void enable() {this->lightning_buffer->enable();}
  void disable() {this->lightning_buffer->disable();}
};
#endif
