#include "firework.hpp"

Firework::Firework() : Object(), Movable() {
  this->fwb = new FireworkBuffer();
  if(this->fwb == NULL) {
    assert(0); //Out of memory! :(
  }
  this->add_buffer(this->fwb);
}
