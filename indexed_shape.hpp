#ifndef INDEXED_SHAPE_HPP
#define INDEXED_SHAPE_HPP

#include <vector>

#include "shape.hpp"


class IndexedShape : public Shape {
private:
std::vector<GLfloat> vertex_data;
std::vector<GLuint> index_data;
std::vector<GLuint> adjacency_data;
protected:
  virtual Mesh * create_mesh();
public:
  virtual ~IndexedShape() {}
IndexedShape(std::vector<GLfloat> & vertex_data, std::vector<GLuint> & index_data,std::vector<GLuint> & adjacency_data);
};

#endif
