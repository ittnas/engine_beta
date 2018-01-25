#include "viewer.hpp"

void Viewer::update_phase_function() {
  
}

void Viewer::set_camera(Camera * camera) {
  this->camera = camera;
}

Viewer::Viewer() : Object(),camera(0) {

}
