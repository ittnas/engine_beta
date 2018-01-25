#include "triangle.hpp"

Triangle::Triangle(const glm::vec3 & v0,const glm::vec3 & v1,const glm::vec3 & v2) : Shape() {
  v[0] = v0;
  v[1] = v1;
  v[2] = v2;
  set_mesh();
}

Mesh * Triangle::create_mesh() {
  Mesh * mesh = new Mesh(TRIANGLE,INDEXED);
  GLfloat data[3*4];
  for(GLuint ii=0;ii<3;ii++) {
    for(GLuint jj=0;jj<3;jj++) {
      data[ii*4 + jj] = v[ii][jj];
    }
    data[ii*4 + 3] = 1.0f;
  }
  glm::vec3 norm = glm::cross(v[1]-v[0],v[2]-v[0]);
  GLfloat normal_data[3*3];
  for(GLuint ii=0;ii<3;ii++) {
    for(GLuint jj=0;jj<3;jj++) {
      normal_data[ii*3+jj] = norm[jj];
    }
  }
 
  GLfloat tex_coord[3*2] = {
    0,1,
    0,0,
    1,0,
  };

  GLuint index_data[3] = {
    0,1,2,
  };
  mesh->add_attribute_array(4,3,data);
  mesh->add_attribute_array(3,3,normal_data);
  mesh->add_attribute_array(2,3,tex_coord);
  mesh->set_index_array(3,index_data);
  //mesh->convert_to_triangles();
  mesh->create_adjacency_data();
  
  mesh->save_to_memory();
  mesh->save_index_data_to_memory();
  mesh->save_adjacency_data_to_memory(GL_FALSE);
  return mesh;
}
