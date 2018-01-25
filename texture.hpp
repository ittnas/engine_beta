#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>

/*
  Idea is to have different kinds of textures represented by this class.
 */



class Texture {
private:
protected:
  GLint internal_format; //GL_RGBA for instance. Format of texture in glsl.
  GLenum format; // Format of the data, GL_RGBA for instance, if there are four elements for a single pixel.
  GLenum type; //GL_FLOAT. Type of the input data.
  GLenum target;
  GLuint texture_ident;
  Texture(GLint internal_format,GLenum format,GLenum type,GLenum target); //And Format etc.
  virtual GLsizei get_data_length_in_bytes() = 0; //TODO
  GLubyte * copy_data(GLubyte * const source);
public:
  virtual ~Texture() {}
  GLuint get_texture_ident() const {return texture_ident;}
  GLenum get_target() const {return target;}
  virtual void print_texture() {}
};

#endif
