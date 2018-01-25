#ifndef ATTRIBUTE_ARRAY_HPP
#define ATTRIBUTE_ARRAY_HPP

#include <GL/glew.h>

class AttributeArray {
private:
  GLuint nbr_elements_per_vertex;
  GLuint nbr_vertices;
  GLenum normalize;
  GLfloat * data;

public:
  AttributeArray(GLuint nbr_elements_per_vertex,GLuint nbr_vertices,const GLfloat * data, GLenum normalize = GL_FALSE);
  ~AttributeArray();
  GLfloat * get_data() {return data;}
  GLuint get_data_length_in_bytes(GLuint nbr_vertices) {
    return sizeof(GLfloat)*nbr_vertices*nbr_elements_per_vertex;
  }
  GLuint get_data_length_in_bytes() {
    return get_data_length_in_bytes(this->nbr_vertices);
  }
  GLuint get_vertex_length_in_bytes() {
    return sizeof(GLfloat)*nbr_elements_per_vertex;
  }
  GLenum should_normalize() {
    return normalize;
  }
  GLuint get_nbr_vertices() {return nbr_vertices;}
  GLuint get_nbr_elements_per_vertex() {return nbr_elements_per_vertex;}
  GLuint append_data(GLuint nbr_vertices, const GLfloat * new_data);
};


#endif
