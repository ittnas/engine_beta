#ifndef TEXT_OBJECT_HPP
#define TEXT_OBJECT_HPP

#include <ft2build.h>
#include FT_FREETYPE_H //This is a weird form of a macro definition. It is required to get freetype work correctly.
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <unordered_map>

#include "object.hpp"
#include "movable.hpp"
#include "character_object.hpp"


class TextObject : virtual public Object, virtual public Movable {
private:
  static FT_Library ft;
  static GLboolean library_initialized;
  FT_GlyphSlot g;
  FT_Face face;
  std::unordered_map<char,Texture2D *> character_textures;
  std::unordered_map<char,std::vector<CharacterObject *> > character_objects;
  std::unordered_map<char,GLint> nbr_chars_stored;
  const GLint internal_format = GL_ALPHA;
  const GLenum format = GL_ALPHA;
  const GLenum type = GL_UNSIGNED_BYTE;
  const GLuint default_font_size_in_pixels = 64;
  GLuint font_size_in_pixels;
  //std::string current_text;
protected:
public:
  TextObject();
  virtual ~TextObject();
  void render_text(std::string text,GLfloat x, GLfloat y);
  virtual void set_to_default_text(); // Sets the default text
  void set_text(std::string text);
  void set_text(std::string text,GLuint font_size_in_pixels);
  void set_font_size(GLuint size_in_pixels);
};

#endif
