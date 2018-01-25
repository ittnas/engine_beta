#include<cstring>
#include<iostream>

#include "index_array.hpp"

IndexArray::IndexArray(GLuint nbr_vertices, GLuint * data) : nbr_vertices(nbr_vertices), index_data(data) {
  this->index_data = (GLuint*)malloc(nbr_vertices*sizeof(GLuint));
  if(!this->index_data) {
    std::cerr << "Failed to allocata memory for data." << std::endl;
  }
  memcpy(this->index_data,data,nbr_vertices*sizeof(GLuint));
}
