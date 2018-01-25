#ifndef VIEWER_HPP
#define VIEWER_HPP

#include "object.hpp"
#include "camera.hpp"

/* NOT FINISHED */

class Viewer : virtual public Object {
public:
  Viewer();
  virtual ~Viewer() {}
  void set_camera(Camera * camera);
protected:
  void update_phase_function();
private:
  Camera * camera; // This is the camera that looks to the other world.
};

#endif
