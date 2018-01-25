#include "killer_action.hpp"
#include "firework.hpp"

KillerAction::KillerAction(GLint life_time,GLint stop, GLint start) : Action(stop,start), life_time(life_time) {
  
}

GLvoid KillerAction::perform_action(Object * target, GLint time) {
  Firework * fw = dynamic_cast<Firework *>(target);
  if(fw->get_age((GLfloat)time/1000)*1000 > life_time) {
    delete fw;
  }
}
