#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "lightning_buffer.hpp"
#include "movable.hpp"
#include "gl_constants.hpp"

LightningBuffer::LightningBuffer() : Buffer("light_buffer",buffer_length), intensity(glm::vec4(1,1,1,1)),exponent(64.0),cutoff(360),kc(1.0),kl(0.0),kq(0.0), is_active(GL_TRUE), light_index(0) {
  
}

void LightningBuffer::update_buffer_phase1(Object * parent) {
  Movable * parent_object = dynamic_cast<Movable *>(parent);
  glm::vec4 position = parent_object->get_position_global();
  //std::cout << "global position (from lightning_buffer.cpp):" << position[0] << "," <<position[1] << "," <<position[2]<< "," <<position[3]  << std::endl;
  glm::vec3 direction = glm::mat3_cast(parent_object->get_orientation())*glm::vec3(0,0,-1);
  assert(light_index < MAX_NBR_LIGHTS);
  GLuint position_in_bytes = buffer_length*light_index/MAX_NBR_LIGHTS;
  glBindBuffer(GL_UNIFORM_BUFFER,get_ubo_pointer());
  glBufferSubData(GL_UNIFORM_BUFFER,position_in_bytes,sizeof(position),glm::value_ptr(position));
  position_in_bytes += sizeof(position);
  glBufferSubData(GL_UNIFORM_BUFFER,position_in_bytes,sizeof(intensity),glm::value_ptr(intensity));
  position_in_bytes += sizeof(intensity);
  glBufferSubData(GL_UNIFORM_BUFFER,position_in_bytes,sizeof(direction),glm::value_ptr(direction));
  position_in_bytes += sizeof(direction);
  glBufferSubData(GL_UNIFORM_BUFFER,position_in_bytes,sizeof(exponent),&exponent);
  position_in_bytes += sizeof(exponent);
  glBufferSubData(GL_UNIFORM_BUFFER,position_in_bytes,sizeof(cutoff),&cutoff);
  position_in_bytes += sizeof(cutoff);
  glBufferSubData(GL_UNIFORM_BUFFER,position_in_bytes,sizeof(kc),&kc);
  position_in_bytes += sizeof(kc);
  glBufferSubData(GL_UNIFORM_BUFFER,position_in_bytes,sizeof(kl),&kl);
  position_in_bytes += sizeof(kl);
  glBufferSubData(GL_UNIFORM_BUFFER,position_in_bytes,sizeof(kq),&kq);
  position_in_bytes += sizeof(kq);
  glBufferSubData(GL_UNIFORM_BUFFER,position_in_bytes,sizeof(is_active),&is_active);
  position_in_bytes += sizeof(is_active);
  /*  GLbyte * debug_data = (GLbyte *)malloc(16);
  GLbyte * position_data = (GLbyte *)glm::value_ptr(position);
  glGetBufferSubData(GL_UNIFORM_BUFFER,0,16,debug_data);
  free(debug_data);*/
  glBindBuffer(GL_UNIFORM_BUFFER,0);
}
