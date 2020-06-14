#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include "uniform_material_buffer.hpp"

UniformMaterialBuffer::UniformMaterialBuffer() : Buffer("uniform_material_buffer",buffer_length),ambient_color(0,0,0,1),diffusive_color(0,0,0,1),specular_color(0,0,0,1),shininess(16) {
  
}

void UniformMaterialBuffer::update_buffer_phase2(Object * parent) {
  glBindBuffer(GL_UNIFORM_BUFFER,get_ubo_pointer());
  glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(ambient_color),glm::value_ptr(ambient_color));
  glBufferSubData(GL_UNIFORM_BUFFER,16,sizeof(diffusive_color),glm::value_ptr(diffusive_color));
  glBufferSubData(GL_UNIFORM_BUFFER,32,sizeof(specular_color),glm::value_ptr(specular_color));
  glBufferSubData(GL_UNIFORM_BUFFER,48,sizeof(shininess),&shininess);
  glBindBuffer(GL_UNIFORM_BUFFER,0);
}
