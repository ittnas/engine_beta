#ifndef TEXTURE_UNIFORM_CONTAINER_HPP
#define TEXTURE_UNIFORM_CONTAINER_HPP

#include <unordered_map>
#include <GL/glew.h>
#include <string>
#include <utility>

#include "uniform_container.hpp"
#include "texture.hpp"

class TextureUniformContainer : public UniformContainer {
private:
  std::unordered_map<std::string,std::pair<Texture *,GLuint>> textures;
public:
  TextureUniformContainer();
  virtual ~TextureUniformContainer() {}
  void pre_draw_function();
  void update_uniforms(Object * parent);
  void add_texture(std::string texture_name,Texture * texture,GLuint texture_unit);
  void remove_texture(std::string texture_name);
  void clear_textures();
};
#endif
