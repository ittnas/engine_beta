#include "texture_uniform_container.hpp"
#include "uniform_list.hpp"

TextureUniformContainer::TextureUniformContainer() : UniformContainer() {
  
}

void TextureUniformContainer::pre_draw_function() {
  for(auto texture_pair : textures) {
    glActiveTexture(GL_TEXTURE0 + texture_pair.second.second);
    glBindTexture(texture_pair.second.first->get_target(),texture_pair.second.first->get_texture_ident());
  }
}

void TextureUniformContainer::update_uniforms(Object * parent) {
  for(auto texture_pair : textures) {
    glUniform1i(UniformList::get_uniform_location(texture_pair.first),texture_pair.second.second);
  }
}

void TextureUniformContainer::add_texture(std::string texture_name,Texture * texture, GLuint texture_unit) {
  textures[texture_name] = std::pair<Texture *, GLuint>(texture,texture_unit);
}

void TextureUniformContainer::remove_texture(std::string texture_name) {
  textures.erase(texture_name);
}

void TextureUniformContainer::clear_textures() {
  textures.clear();
}





