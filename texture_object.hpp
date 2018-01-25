#ifndef TEXTURE_OBJECT_HPP
#define TEXTURE_OBJECT_HPP

#include "object.hpp"
#include "texture_buffer.hpp"

class TextureObject : virtual public Object {
private:
  TextureBuffer * texture_buffer;
public:
  TextureObject();
  virtual ~TextureObject() {}
};
#endif
