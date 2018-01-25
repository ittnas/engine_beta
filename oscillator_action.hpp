#ifndef OSCILLATOR_ACTION_HPP
#define OSCILLATOR_ACTION_HPP

#include "action.hpp"
#include "movable.hpp"

class OscillatorAction : public Action {
private:
  GLfloat f;
  GLfloat A;
  glm::vec3 axis_of_motion;
public:
  OscillatorAction(GLfloat f, GLfloat A, glm::vec3 axis_of_motion,GLint stop, GLint start);
  virtual ~OscillatorAction() {}
  virtual void perform_action(Object * target,GLint time);
};

#endif
