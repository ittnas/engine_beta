#ifndef LIGHTABLE_HPP
#define LIGHTABLE_HPP

#include "lightning_buffer.hpp"
#include "object.hpp"
#include "movable.hpp"

class Lightable : virtual public Object, virtual public Movable {
private:
  LightningBuffer * lightning_buffer;
public:
  Lightable();
};

#endif
