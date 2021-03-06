#ifndef SHAPE_HPP
#define SHAPE_HPP
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "mesh.hpp"

class Shape {
private:
  Mesh * mesh;
protected:

  virtual Mesh * create_mesh() = 0;
public:
  Shape();
  //void set_drawer() {this->drawer = create_drawer();}
  void set_mesh() {this->mesh = create_mesh();}
  virtual void draw(DrawingMode drawing_mode = NOTSET);
  static void add_to_vector(std::vector<GLfloat> & output,glm::vec4 & target);
  Mesh * get_mesh() {return mesh;}
  virtual ~Shape();
  //Mesh * create_mesh();
  //void create_adjacency_data() {if(mesh!=NULL) {mesh->create_adjacency_data;save_adjacency_data_to_memory(GL_TRUE);}
};

#endif
