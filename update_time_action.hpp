#ifndef UPDATE_TIME_ACTION_HPP
#define UPDATE_TIME_ACTION_HPP

#include "action.hpp"

class UpdateTimeAction : public Action {
private:
  
public:
  UpdateTimeAction(GLint stop = -1, GLint start = -1);
  virtual ~UpdateTimeAction() {}
  virtual void perform_action(Object * target, GLint time);
};

#endif
