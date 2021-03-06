#include <iostream>
#include "oscillator_action.hpp"

OscillatorAction::OscillatorAction(GLfloat f, GLfloat A, glm::vec3 axis_of_motion,GLint stop, GLint start) : Action(stop,start), f(f),A(A),axis_of_motion(axis_of_motion) {}

void OscillatorAction::perform_action(Object * target, GLint time) {
  Movable * movable = dynamic_cast<Movable *>(target);
  movable->set_position(glm::vec4(A*sin(2*3.1415925*f*(GLfloat)time/1000)*axis_of_motion.x,A*sin(2*3.1415925*f*(GLfloat)time/1000)*axis_of_motion.y,A*sin(2*3.1415925*f*(GLfloat)time/1000)*axis_of_motion.z,1.0));
  //std::cout << movable->get_position().z << std::endl;
}
