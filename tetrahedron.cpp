#include "tetrahedron.hpp"

Tetrahedron::Tetrahedron(GLfloat size) : Shape(),size(size) {
  set_mesh();
}

Mesh * Tetrahedron::create_mesh() {
  Mesh * mesh = new Mesh(TRIANGLE,INDEXED);
  //GLfloat * data[4*4]; //4 data points
  GLfloat data[4*4] = {
    size,0,-SQRT2*size,1.0,
    -size,0,-SQRT2*size,1.0,
    0,size,SQRT2*size,1.0,
    0,-size,SQRT2*size,1.0
  };
  /*
  GLfloat data[4*4] = {
    size,-size,size,1.0,
    size,size,-size,1.0,
    -size,-size,-size,1.0,
    -size,size,size,1.0,
  };
  */

  //WRONG!
  // GLfloat normal_data[4*3] = {
  //   0,0,-1,
  //   0,-1,0,
  //   -1,0,0
  // };

  GLfloat normal_data[4*3] = {
    1,1,1,
    1,-1,-1,
    -1,1,-1,
    -1,-1,1
  };
  for(GLint ii =0;ii<12;ii++) {
  normal_data[ii] = normal_data[ii]/sqrt(3);
  }
  GLfloat tex_coord[4*2] = {
    0,0,
    0,1,
    1,0,
    1,1
  };
  /*
    GLuint index_data[4*3] = {
    3,0,2,
    0,1,2,
    3,2,1,
    1,3,0
    };
  */
  /*
    GLuint index_data[4*3] = {
      0,1,2,
      3,2,1,
      2,3,0,
      1,0,3
    };
  */
    GLuint index_data[4*3] = {
      2,1,0,
      1,2,3,
      0,3,2,
      3,0,1
    };
    /*
  GLuint index_data[4*3] = {
      0,1,3,
      0,2,1,
      2,3,1,
      3,2,0
    };
    */

  
  mesh->add_attribute_array(4,4,data);
  mesh->add_attribute_array(3,4,normal_data);
  mesh->add_attribute_array(2,4,tex_coord);
  mesh->set_index_array(12,index_data);

  mesh->create_adjacency_data();

  mesh->save_to_memory();
  mesh->save_index_data_to_memory();
  mesh->save_adjacency_data_to_memory(GL_FALSE);
  return mesh;
}
