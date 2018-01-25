#ifndef LIGHTABLE_TEXT_HPP
#define LIGHTABLE_TEXT_HPP

#include "object.hpp"
#include "uniform_material.hpp"
#include "text_object.hpp"

class LightableText : virtual public Object, virtual public UniformMaterial, virtual public TextObject {
public:
  LightableText() : Object(), UniformMaterial(), TextObject() {}
  virtual ~LightableText() {}
private:
};

#endif
