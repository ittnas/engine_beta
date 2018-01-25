#include <cstring>
#include <assert.h>
#include "texture.hpp"

Texture::Texture(GLint internal_format,GLenum format,GLenum type, GLenum target) : internal_format(internal_format), format(format), type(type), target(target) {
  glGenTextures(1,&texture_ident); // Generates new ident for the texture
  glBindTexture(target,texture_ident); // and to darkness binds it.

  // Texture is actually loaded into the memory by inheriting classes.
}

GLubyte * Texture::copy_data(GLubyte * const source) {
  assert(source);
  GLsizei nbr_bytes_to_copy = get_data_length_in_bytes();
  GLubyte * destination = new GLubyte[nbr_bytes_to_copy];
  memcpy(destination,source,nbr_bytes_to_copy);
  return destination;
}
