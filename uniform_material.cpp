#include "uniform_material.hpp"

UniformMaterial::UniformMaterial() : Object(), Movable() {
  this->uniform_material_buffer = new UniformMaterialBuffer();
  if(this->uniform_material_buffer == 0) {
    assert(0); //OoM
  }
  this->add_buffer(this->uniform_material_buffer);
}
