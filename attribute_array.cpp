#include <cstdlib>
#include <iostream>
#include <cstring>

#include "attribute_array.hpp"

AttributeArray::AttributeArray(GLuint nbr_elements_per_vertex,GLuint nbr_vertices,const GLfloat * data, GLenum normalize) : nbr_elements_per_vertex(nbr_elements_per_vertex), nbr_vertices(nbr_vertices), normalize(normalize) {
  this->data = (GLfloat*)malloc(get_data_length_in_bytes());
  if(!this->data) {
    std::cerr << "Failed to allocata memory for data." << std::endl;
  }
  memcpy(this->data,data,get_data_length_in_bytes());
}

AttributeArray::~AttributeArray() {
  free(data);
}

GLuint AttributeArray::append_data(GLuint nbr_vertices, const GLfloat * new_data) {
  GLbyte * new_location = (GLbyte*)realloc(this->data, get_data_length_in_bytes(nbr_vertices + this->nbr_vertices));
  if(!new_location) {
    std::cerr << "Error reallocating memory." << std::endl;
    return 0;
  }
  memcpy(new_location + get_data_length_in_bytes(),new_data,get_data_length_in_bytes(nbr_vertices));
  this->data = (GLfloat*)new_location;
  this->nbr_vertices += nbr_vertices;
  return 1;
}
