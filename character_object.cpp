#include <glm/glm.hpp>

#include "character_object.hpp"
#include "rectangle.hpp"
//#include "parametric_plot.hpp"
//#include "parametric_functions.hpp"

CharacterObject::CharacterObject() : Object(), Movable() {
  /*GLfloat PI2 = 6.2831853;
    ParametricPlot * shape = new ParametricPlot(ParametricFunctions::parametric_torus,glm::vec2(0,PI2),glm::vec2(0,PI2),21,21,GL_TRUE,GL_TRUE);*/
  Shape * shape = new Rectangle(1.0,1.0);
  this->set_shape(shape);
  this->texture_uniform_container = new TextureUniformContainer();
  this->add_uniform_container(this->texture_uniform_container);
}

void CharacterObject::init(Texture2D * texture, GLsizei width, GLsizei rows, GLfloat xpos, GLfloat ypos,GLuint font_size_in_pixels) {
  this->texture_uniform_container->add_texture("character_texture",texture,0);
  this->set_scale(glm::vec3((GLfloat)width/font_size_in_pixels,(GLfloat)rows/font_size_in_pixels,1));
  set_character_position(xpos,ypos,font_size_in_pixels);
  //this->set_position(glm::vec4(xpos/font_size_in_pixels,ypos/font_size_in_pixels,0,1));
  /*
    this->width = width;
    this->rows = rows;
    this->xpos = xpos;
    this->ypos = ypos;
  */
}

void CharacterObject::set_character_position(GLfloat xpos,GLfloat ypos,GLuint font_size_in_pixels) {
  this->set_position(glm::vec4(xpos/font_size_in_pixels,ypos/font_size_in_pixels,0,1));
  //std::cout << "Character position x:" << xpos/font_size_in_pixels << ", y:" << ypos/font_size_in_pixels << std::endl;
}

CharacterObject::~CharacterObject() {
  delete this->shape;
  delete this->texture_uniform_container;
}

