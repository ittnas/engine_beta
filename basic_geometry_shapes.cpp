#include "basic_geometry_shapes.hpp"

RayShape::RayShape(const glm::vec3 & a, const glm::vec3 & d) : Shape(), a(a), d(d) {
  set_mesh();
}

Mesh * RayShape::create_mesh() {
  Mesh * mesh = new Mesh(LINES,INDEXED);
  GLfloat data[2*4] = {a[0],a[1],a[2],1.0f,
		       a[0]+d[0],a[1]+d[1],a[2]+d[2],1.0f};
  GLuint index_data[2] = {
    0,1
  };
  mesh->add_attribute_array(4,2,data);
  mesh->set_index_array(2,index_data);
  //mesh->convert_to_triangles();
  //mesh->create_adjacency_data();
  
  mesh->save_to_memory();
  mesh->save_index_data_to_memory();
  //mesh->save_adjacency_data_to_memory(GL_FALSE);
  return mesh;
}

PointCloud::PointCloud(const std::vector<glm::vec4> & points) : Shape() {
  this->points.reserve(points.size());
  for(GLuint ii=0;ii<points.size();ii++) {
    this->points.push_back(points[ii]);
  }
  set_mesh();
}

Mesh * PointCloud::create_mesh() {
    Mesh * mesh = new Mesh(POINTS,ARRAY);
    GLuint nbr_points = points.size();

    GLfloat data[nbr_points*4];
    for(GLuint ii=0;ii<nbr_points;ii++) {
      for(GLuint jj=0;jj<4;jj++) {
	data[ii*4+jj] = points[ii][jj];
      }
    }

    mesh->add_attribute_array(4,nbr_points,data);
  //mesh->convert_to_triangles();
  //mesh->create_adjacency_data();
  
    mesh->save_to_memory();
    //mesh->save_adjacency_data_to_memory(GL_FALSE);
    return mesh;
}
