#include "update_time_action.hpp"
#include "world.hpp"

UpdateTimeAction::UpdateTimeAction(GLint stop, GLint start) : Action(stop,start) {}

void UpdateTimeAction::perform_action(Object * target, GLint time) {
  World * world = dynamic_cast<World *>(target);
  world->set_time(((GLfloat)time)/1000);
}
