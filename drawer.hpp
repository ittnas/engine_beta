#ifndef DRAWER_HPP
#define DRAWER_HPP

#include "object.hpp"

class Drawer {
public:
  Drawer();
  virtual ~Drawer() {}
  virtual void pre_draw_function(Object * object) = 0;
  virtual void post_draw_function(Object * object) = 0;
};

#endif
