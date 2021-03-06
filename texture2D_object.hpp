#ifndef TEXTURE2D_OBJECT_HPP
#define TEXTURE2D_OBJECT_HPP

#include <assert.h>
#include <GL/glew.h>

#include "object.hpp"
#include "movable.hpp"
#include "texture_uniform_container.hpp"
#include "uniform_material.hpp"


class Texture2DObject : virtual public Object, virtual public Movable, virtual public UniformMaterial  {
private:
  TextureUniformContainer * texture_uniform_container;
  void create_texture(GLubyte * texture, GLsizei height, GLsizei width);
public:
  Texture2DObject();
  virtual ~Texture2DObject() {}
};

#endif
