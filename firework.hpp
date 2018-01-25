#ifndef FIREWORK_HPP
#define FIREWORK_HPP

#include "object.hpp"
#include "firework_buffer.hpp"
#include "movable.hpp"

class Firework : virtual public Object, virtual public Movable {
private:
  FireworkBuffer * fwb;
public:
  Firework();
  virtual ~Firework() {}
  GLvoid set_creation_time(GLfloat time) {this->fwb->set_creation_time(time);}
  GLvoid set_explosion_time(GLfloat time) {this->fwb->set_explosion_time(time);}
  GLfloat get_creation_time() const {this->fwb->get_creation_time();}
  GLvoid set_initial_velocity(glm::vec3 initial_velocity) {this->fwb->set_initial_velocity(initial_velocity);}
  GLfloat get_age(GLfloat time) {return time - get_creation_time();}
};

#endif
