#ifndef ACTION_HPP
#define ACTION_HPP

#include <GL/glew.h>

class Object;

class Action {
private:
  GLint start;
  GLint stop;
public:  
  Action(GLint stop = -1,GLint start = -1) : start(start), stop(stop) {};
  virtual void act(Object * target,GLint time);
  GLint get_start() const {return start;}
  GLint get_stop() const {return stop;}
  virtual ~Action() {}
  virtual void perform_action(Object * target,GLint time) = 0;
  void set_start(GLint start) {
    this->start = start;
  }
  void set_stop(GLint stop) {
    this->stop = stop;
  }
};

#endif
