#ifndef INDEX_ARRAYV2_HPP
#define INDEX_ARRAYV2_HPP

#include <cstdlib>

#include <GL/glew.h>

class IndexArray {
private:
  GLuint * index_data;
  GLuint nbr_vertices;

public:
  IndexArray(GLuint nbr_vertices, GLuint * data);
  GLuint * get_data() {return index_data;}
  GLuint get_nbr_vertices() {return nbr_vertices;}
  ~IndexArray() {free(index_data);}
  GLuint get_buffer_size() {return sizeof(GLuint)*nbr_vertices;}
};

#endif
