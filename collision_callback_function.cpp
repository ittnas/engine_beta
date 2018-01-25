#include "collision_callback_function.hpp"
#include "uniform_color_collision_object.hpp"

void MaterialColorCollisionCallbackFunction::apply(const CollisionPair & cp) {
  ucco->set_ambient_color(ucco->get_ambient_color() + glm::vec4(0.002,0.0,0.0,0.0));
  if(ucco->get_ambient_color().x > 1) {
    glm::vec4 color = ucco->get_ambient_color();
    color.x = 0;
    ucco->set_ambient_color(color);
  }
}
