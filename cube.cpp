#include "cube.hpp"

Cube::Cube(GLfloat size) : Shape(),size(size) {
  set_mesh();
}

Mesh * Cube::create_mesh() {
  Mesh * mesh = new Mesh(QUAD,INDEXED);
  GLfloat hs = size/2;
  GLfloat sqrt3 = 0.57735026;
  GLfloat data[8*4] = {
    -hs,-hs,-hs,1,
    hs,-hs,-hs,1,
    hs,-hs,hs,1,
    -hs,-hs,hs,1,
    -hs,hs,-hs,1,
    hs,hs,-hs,1,
    hs,hs,hs,1,
    -hs,hs,hs,1
  };
  GLfloat normal_data[8*3] = {
    -sqrt3,-sqrt3,sqrt3,
    sqrt3,-sqrt3,sqrt3,
    sqrt3,-sqrt3,-sqrt3,
    -sqrt3,-sqrt3,-sqrt3,
    -sqrt3,sqrt3,sqrt3,
    sqrt3,sqrt3,sqrt3,
    sqrt3,sqrt3,-sqrt3,
    -sqrt3,sqrt3,-sqrt3,
  };

  /*
  GLuint index_data[6*4] = {
    0,3,2,1,
    1,2,6,5,
    0,1,5,4,
    0,4,7,3,
    4,5,6,7,
    3,7,6,2
    };
  */

  GLuint index_data[6*4] = {
    0,1,2,3,
    1,5,6,2,
    0,4,5,1,
    0,3,7,4,
    4,7,6,5,
    7,3,2,6
  };
  mesh->add_attribute_array(4,8,data);
  mesh->add_attribute_array(3,8,normal_data);
  mesh->set_index_array(24,index_data);
  mesh->convert_to_triangles();
  mesh->save_to_memory();
  mesh->save_index_data_to_memory();
  return mesh;
}
