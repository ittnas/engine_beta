#ifndef FIREWORK_BUFFER_HPP
#define FIREWORK_BUFFER_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "buffer.hpp"

class FireworkBuffer : public Buffer {
private:
  static const GLuint buffer_length = 4*3 + 2*4;
  GLfloat creation_time;
  GLfloat explosion_time;
  glm::vec3 initial_velocity;

public:
  virtual void update_buffer_phase2(Object * parent);
  FireworkBuffer();
  virtual ~FireworkBuffer() {}
  GLvoid set_creation_time(GLfloat time) {creation_time = time;}
  GLvoid set_explosion_time(GLfloat time) {explosion_time = time;}
  GLfloat get_creation_time() const {return creation_time;}
  GLvoid set_initial_velocity(glm::vec3 initial_velocity) {this->initial_velocity = initial_velocity;}
};

#endif
