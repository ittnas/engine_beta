#include <random>

#include "firework_action.hpp"
#include "firework_shooter.hpp"

FireworkAction::FireworkAction(GLint stop, GLint start) : Action(stop,start), last_shot(0), delay(1000) {
  
}

void FireworkAction::perform_action(Object * target, GLint time) {
  if(time - last_shot - delay > 0) {
    FireworkShooter * fws = dynamic_cast<FireworkShooter *>(target);
    fws->fire_next((GLfloat)time/1000);
    delay = rand() % 500;
    last_shot = time;
  }
}
