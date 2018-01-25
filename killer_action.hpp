#ifndef KILLER_ACTION_HPP
#define KILLER_ACTION_HPP

#include "action.hpp"

class KillerAction : public Action {
private:
  GLint life_time;
public:
  KillerAction(GLint life_time,GLint stop,GLint start);
  virtual ~KillerAction() {}
  virtual GLvoid perform_action(Object * target, GLint time);
};

#endif
