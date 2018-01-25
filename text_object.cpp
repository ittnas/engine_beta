#include <iostream>
#include <string>
 
#include "text_object.hpp"
#include "oscillator_action.hpp"

GLboolean TextObject::library_initialized = GL_FALSE;
FT_Library TextObject::ft = 0;
/*
const GLint TextObject::internal_format = GL_ALPHA;
const GLenum TextObject::format = GL_ALPHA;
const GLenum TextObject::type = GL_UNSIGNED_BYTE;
*/

TextObject::TextObject() : Object(), Movable() {
  if(!library_initialized) {
    if(FT_Init_FreeType(&ft)) {
      std::cerr << "Could not init freetype library." << std::endl;
      assert(0); //Failed to initialize
    }
  }
  if(FT_New_Face(ft, "/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf",0, &face)) {
    std::cerr << "Could not open font." << std::endl;
    assert(0);
  }
  this->font_size_in_pixels = default_font_size_in_pixels;
  FT_Set_Pixel_Sizes(face, 0, default_font_size_in_pixels);
  //FT_Set_Char_Size(face, 0, default_font_size_in_pixels*100,72,72);
  //std::cout << "Init successful!" << std::endl;
  g = face->glyph;
}

void TextObject::set_text(std::string text,GLuint font_size_in_pixels) {
  this->font_size_in_pixels = font_size_in_pixels;
  FT_Set_Pixel_Sizes(face, 0, font_size_in_pixels);
  set_text(text);
}

void TextObject::set_text(std::string text) {
  clear_children(); // Clear all the characters
  GLfloat x = 0;
  GLfloat y = 0;
  std::unordered_map<char,GLint> nbr_chars;
#ifdef DEBUG
  std::cout << "Writing text:" << text << std::endl;
#endif
  for(GLsizei i = 0,length = text.size(); i < length; i++) {
    char character = text[i];
    auto iter = character_textures.find(character);    
    GLint char_load_success = FT_Load_Char(face, character,FT_LOAD_RENDER);
    assert(!char_load_success);
    nbr_chars[character] = nbr_chars[character] + 1;
    if(g->bitmap.buffer != 0) {
      if(iter == character_textures.end()) {
	Texture2D * texture = new Texture2D(g->bitmap.buffer,internal_format,format,type,g->bitmap.width,g->bitmap.rows);
	character_textures[character] = texture;
#ifdef DEBUG
	std::cout << "From text_object.cpp: Char = \"" << character << "\", width = " << g->bitmap.width << ",height = " << g->bitmap.rows << "." << std::endl;
#endif
	texture->print_texture();
	// If the texture for the character is not found, then there cannot be any character objects either. Thus we construct one.
	CharacterObject * chobj = new CharacterObject();
	chobj->set_render_flag(get_render_flag());
	chobj->init(character_textures[character],g->bitmap.width,g->bitmap.rows,x+g->bitmap_left,y-g->bitmap.rows + g->bitmap_top,this->font_size_in_pixels);
	character_objects[character].push_back(chobj);
	this->add_child(chobj);
	nbr_chars_stored[character] = nbr_chars_stored[character] + 1;
      } else if(nbr_chars[character] > nbr_chars_stored[character]) { // We might have to construct one more character objects.
	CharacterObject * chobj = new CharacterObject();
	chobj->init(character_textures[character],g->bitmap.width,g->bitmap.rows,x+g->bitmap_left,y-g->bitmap.rows + g->bitmap_top,this->font_size_in_pixels);
	chobj->set_render_flag(get_render_flag());
	this->add_child(chobj);
	nbr_chars_stored[character] = nbr_chars_stored[character] + 1;
	character_objects[character].push_back(chobj);
      } else { // Or then one is already found and we can use it
#ifdef DEBUG
	std::cout << "nbr_chars [" << &character << "] stored:" << nbr_chars_stored[character] << ", needed: " << nbr_chars[character] <<  std::endl;
#endif
	CharacterObject * chobj = character_objects[character][nbr_chars[character]-1];
	chobj->set_character_position(x+g->bitmap_left,y-g->bitmap.rows + g->bitmap_top,this->font_size_in_pixels);
	this->add_child(chobj);
      }
      x += g->advance.x >> 6;
      y += g->advance.y >> 6;
    } else { // Indicates space
      x += this->font_size_in_pixels/4;
    }
  }
}

void TextObject::set_font_size(GLuint size_in_pixels) {
  // TODO regenerate textures to correspons to the new font size
}

/* Depracedted. Can be used to directly draw text to the screen. */
void TextObject::render_text(std::string text,GLfloat x, GLfloat y) {
  for(GLuint i = 0; i < text.length();i++) {
    FT_Load_Char(face, text.at(i),FT_LOAD_RENDER);
    /*GLfloat x2 = x + g->bitmap_left*sx;
    GLfloat y2 = -y - g->bitmap_top*sy;
    GLfloat w = g->bitmap.width*sx;
    */
    /*for(GLuint j = 0; j<g->bitmap.width*g->bitmap.rows;j++) {
      std::cout << g->bitmap.buffer[j];
      }
    std::cout << std::endl;
    */
    glRasterPos2i(x,y);
    glDrawPixels(g->bitmap.width,g->bitmap.rows,GL_RED,GL_BITMAP,g->bitmap.buffer);
    x = x+(g->advance.x >> 6);
    y = y+(g->advance.y >> 6);
  }
}

TextObject::~TextObject() {
  for(auto character : character_textures) {
    delete character.second;
  }
}

void TextObject::set_to_default_text() {
  set_text("Default.");
}
