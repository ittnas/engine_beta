#ifndef FIREWORK_SHOOTER_HPP
#define FIREWORK_SHOOTER_HPP

#include "object.hpp"
#include "movable.hpp"
#include "firework_shape.hpp"
#include "program.hpp"
#include "killer_action.hpp"

class FireworkShooter : virtual public Object, virtual public Movable  {
private:
  std::vector<FireworkShape *> fireworks;
  GLuint current_index;
  Program * fwprog;
  GLuint n;
  KillerAction * killer_action;
public:
  FireworkShooter();
  virtual ~FireworkShooter() {}
  GLvoid fire_next(GLfloat time);
};

#endif
