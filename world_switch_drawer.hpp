#ifndef WORLD_SWITCH_DRAWER_HPP
#define WORLD_SWITCH_DRAWER_HPP

#include "drawer.hpp"

class WorldSwitchDrawer : Drawer {
private:
  Camera * camera; // The camera this drawer is attached to.
public:
  WorldSwitchDrawer(Camera * camera) {}
  virtual ~WorldSwitchDrawer() {}
  virtual void pre_draw_function(Object * object);
  virtual void post_draw_function(Object * object) {}

};

#endif
