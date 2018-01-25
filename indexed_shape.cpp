#include "indexed_shape.hpp"

IndexedShape::IndexedShape(std::vector<GLfloat> & vertex_data, std::vector<GLuint> & index_data, std::vector<GLuint> & adjacency_data) : vertex_data(vertex_data), index_data(index_data), adjacency_data(adjacency_data) {
  set_mesh();
}

Mesh * IndexedShape::create_mesh() {
  Mesh * mesh = new Mesh(TRIANGLE,INDEXED);
  GLfloat * data = &vertex_data[0];
  GLfloat normal_data[vertex_data.size()/4*3] = {0}; 
  GLfloat tex_coord[vertex_data.size()/2] = {0};
  GLuint * i_data = &index_data[0];
  mesh->add_attribute_array(4,vertex_data.size()/4,data);
  mesh->add_attribute_array(3,vertex_data.size()/4,normal_data);
  mesh->add_attribute_array(2,vertex_data.size()/4,tex_coord);
  mesh->set_index_array(index_data.size(),i_data);

  mesh->save_to_memory();
  mesh->save_index_data_to_memory();
  //mesh->create_adjacency_data();
  mesh->insert_adjacency_array(adjacency_data);
  mesh->save_adjacency_data_to_memory(GL_FALSE);

  return mesh;

}
