#ifndef TEXTURE_BUFFER_HPP
#define TEXTURE_BUFFER_HPP

#include <vector>
#include "buffer.hpp"
#include "texture.hpp"

//THIS IS DEPRACATED, replaced with TextureUniformContainer

class TextureBuffer : public Buffer {
private:
  std::vector<Texture *> textures; // Contains all the properties of a texture. //TODO change to unordered_map. Thus unused indices won't be used.
  static const GLuint MAX_NBR_TEXTURES; // Initialize in the initializer list
  static GLuint buffer_length;
public:
  TextureBuffer();
  virtual ~TextureBuffer() {}
  void update_buffer_phase2(Object * parent);
  void update_uniforms(Object * parent);
  void add_texture(Texture * texture) {textures.push_back(texture);}
  void set_texture(Texture * texture, GLsizei index);
  void remove_textures();
  void remove_texture(GLsizei index);
  void pre_draw_function();
  
};

#endif
