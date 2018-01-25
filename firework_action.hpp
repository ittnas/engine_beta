#ifndef FIREWORK_ACTION_HPP
#define FIREWORK_ACTION_HPP

#include "action.hpp"

class FireworkAction : public Action {
private:
  GLint last_shot;
  GLint delay;
public:
  FireworkAction(GLint stop = -1, GLint start = -1);
  virtual ~FireworkAction() {}
  virtual void perform_action(Object * target, GLint time);
};

#endif
