#ifndef KEY_ACTION_HPP
#define KEY_ACTION_HPP

#include "action.hpp"
#include "key_table.hpp"

class KeyAction : public Action {
private:
  KeyTable * key_table;
  GLfloat move_distance;
  GLint previous_tick;
  GLdouble speed; //Units/second
  GLdouble roll_speed; // degree/second
  GLdouble pitch_speed;
  GLdouble yaw_speed;
public:
  KeyAction(GLint stop = -1, GLint start = -1);
  virtual ~KeyAction() {}
  virtual void perform_action(Object * target, GLint time);
};

#endif
