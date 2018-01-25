#ifndef CHARACTER_OBJECT_HPP
#define CHARACTER_OBJECT_HPP

#include "object.hpp"
#include "movable.hpp"
#include "texture2D.hpp"
#include "texture_uniform_container.hpp"

class CharacterObject : virtual public Object, virtual public Movable {
private:
  Texture2D * texture;
  TextureUniformContainer * texture_uniform_container;
  Shape * shape; // Only for destructor
  /*
  GLsizei width,rows;
  GLfloat xpos,ypos;
  */
public:
  CharacterObject();
  virtual ~CharacterObject();
  void init(Texture2D * texture,GLsizei width, GLsizei rows, GLfloat xpos, GLfloat ypos,GLuint font_size_in_pixels);
  void set_character_position(GLfloat xpos,GLfloat ypos,GLuint font_size_in_pixels);
};

#endif
