#include "world_switch_drawer.hpp"

void WorldSwitchDrawer::pre_draw_function(Object * object) {
  World * world = object->get_world();
  assert(world); // World is not set :((
  world->update(); // Switch to new context
  camera->select_render_target(); // Render to frame buffer texture or something.
  world->render();
  camera->get_world()->update(); // Return back to old context.
}
