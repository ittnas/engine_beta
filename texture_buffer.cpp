#include <assert.h>

#include "texture_buffer.hpp"
#include "uniform_list.hpp"

const GLuint TextureBuffer::MAX_NBR_TEXTURES = 20;
GLuint TextureBuffer::buffer_length = sizeof(GLuint)*MAX_NBR_TEXTURES;

TextureBuffer::TextureBuffer() : Buffer("texture_buffer",buffer_length) {
}

void TextureBuffer::update_buffer_phase2(Object * parent) {
  glBindBuffer(GL_UNIFORM_BUFFER,get_ubo_pointer());
  GLuint ii = 0;
  for(auto texture : textures) {
    glBufferSubData(GL_UNIFORM_BUFFER,ii*sizeof(GLuint),sizeof(GLuint),&ii); // ii is the index of the used texture unit.
    ii++;
  }
  glBindBuffer(GL_UNIFORM_BUFFER,0);
}

void TextureBuffer::update_uniforms(Object * parent) {
  glUniform1i(UniformList::get_uniform_location("diffusive_color_sampler"),0); //TODO active texture unit should be allocated as textures are added
}

void TextureBuffer::set_texture(Texture * texture, GLsizei index) {
  if(textures.size() <= index) {
    textures.resize(index+1,0);
  }
  textures.at(index) = texture;
}

void TextureBuffer::remove_textures() {
  textures.clear();
}

void TextureBuffer::remove_texture(GLsizei index) {
  assert(index < textures.size());
  textures.erase(textures.begin() + index);
}

void TextureBuffer::pre_draw_function() {
  for(GLsizei i = 0; i < textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(textures[i]->get_target(),textures[i]->get_texture_ident());
  }
}
