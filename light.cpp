#include "light.hpp"

Light::Light() : Object() {
  this->lightning_buffer = new LightningBuffer();
  if(this->lightning_buffer == NULL) {
    assert(0); // Out of memory! :(
  }
  this->add_buffer(this->lightning_buffer);
}
