#ifndef TEXTURE1D_HPP
#define TEXTURE1D_HPP

#include "texture.hpp"

class Texture1D : public Texture {
private:
  GLsizei width;
  GLubyte * data;
protected:
  GLsizei get_data_length_in_bytes();
public:
  Texture1D(GLubyte * const data,GLint internal_format,GLenum format,GLenum type,GLsizei width);
  virtual ~Texture1D() {delete data;};
  virtual void print_texture();
};
#endif
