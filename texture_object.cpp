#include "texture_object.hpp"

TextureObject::TextureObject() {
  this->texture_buffer = new TextureBuffer();
  if(this->texture_buffer == NULL) {
    assert(0); //Out of memory! :(
  }
  this->add_buffer(this->texture_buffer);
}
