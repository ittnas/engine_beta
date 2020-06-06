#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <map>
#include <assert.h>
#include <exception>
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>
#include <vector>
#include <random>

#include "mesh.hpp"
#include "polygon_type.hpp"
#include "edge.hpp"
#include "bounding_geometry.hpp"
#include "bounding_geometry_generation_algorithms.hpp"
#include "collision_detection_algorithms.hpp"

Mesh::Mesh(PolygonType polygon_type, DrawingMode drawing_mode) : buffer_handle(0), index_buffer_handle(0), vao_handle(0), index_array(0), drawing_mode(drawing_mode), polygon_type(polygon_type), adjacency_array(0) {
}

Mesh::~Mesh() {
  for(GLuint i = 0; i < attribute_arrays.size(); i++) {
    delete attribute_arrays[i];
  }
  delete index_array;
}

void Mesh::save_adjacency_data_to_memory(GLboolean override_index_buffer) {
  /*if(adjacency_buffer_handle) { // override existing adjacence_buffer
    glDeleteBuffers(1,&adjacency_buffer_handle);
    adjacency_buffer_handle = 0;
    }*/

  if(override_index_buffer) {
    //Delete the index buffer and write adjacency data on top of it. All the future drawing calls must now be with adjacency information.
    if(index_buffer_handle) { // If index buffer is empty there is no need to delete it.
      glDeleteBuffers(1,&index_buffer_handle);
      index_buffer_handle = 0;
    }
    //glGenBuffers(1, &adjacency_buffer_handle);
    glGenBuffers(1, &index_buffer_handle);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,adjacency_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,index_buffer_handle);
    GLuint * adjacency_data = adjacency_array; // Should be stored in an IndexArray... Call would then be 
    // GLuint * adjacency_data = adjacency_array->get_data();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,index_array->get_buffer_size()*2,(GLvoid *)adjacency_data,GL_STATIC_DRAW); // If adjacency data is stored in IndexArray, use adjacency_array->get_buffer_size() instead.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
  } else {
    if(!index_buffer_handle) { // Index buffer not created
      return save_adjacency_data_to_memory(GL_TRUE);
    }
    GLuint temp_index_buffer_handle = index_buffer_handle;
    glGenBuffers(1, &index_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,index_buffer_handle);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER,index_array->get_buffer_size()*3,(GLvoid *)index_array->get_data(),GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,index_array->get_buffer_size()*3,(GLvoid *)0,GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,index_array->get_buffer_size(),(GLvoid *) index_array->get_data());
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,index_array->get_buffer_size(),index_array->get_buffer_size()*2,(GLvoid *)adjacency_array);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glDeleteBuffers(1,&temp_index_buffer_handle);
    
    //update vao
    glBindVertexArray(vao_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,index_buffer_handle);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
  }
}

/** 
 * Saves the index data used for indexed drawing into GPU. If index array is not set does nothing. save_to_memory has to be called before this function. Overwrites existing index data.
 */

void Mesh::save_index_data_to_memory() {
  if(index_buffer_handle) { //Overwrite
    glDeleteBuffers(1,&index_buffer_handle);
    index_buffer_handle = 0;
  }
  if(index_array && vao_handle) { //index_array and vao_handle need to be defined.
  glGenBuffers(1, &index_buffer_handle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_handle);
  GLuint * index_data = index_array->get_data();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,index_array->get_buffer_size(),(GLvoid *)index_data,GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

  glBindVertexArray(vao_handle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,index_buffer_handle);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
  }
}

/** 
 * Saves the vertex arrays to GPU's array buffer. If there is already data saved to memory that data is overridden.
 * 
 * @param interleaved Tells whether data should be stored sequantially or interleaved.
 */

void Mesh::save_to_memory(GLboolean interleaved) {
  /* First all data is saved to a buffer. After the data is saved, a vertex array is created and bound. Then the buffer is rebound, signaling the source of data to the vertex array. glEnableVertexAttribArray then tells openGL how many different attributes there are in the data and how they are aligned, thus enabling the primitives to be rendered by simply calling glDrawArrays(...);
     
   */
  this->interleaved = interleaved;
  if(buffer_handle) {
    glDeleteBuffers(1,&buffer_handle);
    buffer_handle = 0;
  }

  if(vao_handle) {
    glDeleteVertexArrays(1,&vao_handle);
    vao_handle=0;
  }
  // Interleaving can be later optimized so that the user can decide which vertex attributes are interleaved with each other. In principle all the vertex attributes that change  at same time (or don't change at all) should be interleaved with each other.
  if(interleaved) {

    glGenBuffers(1,&buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER,buffer_handle);
    GLfloat * data = create_buffer_data(interleaved);
    glBufferData(GL_ARRAY_BUFFER,calculate_buffer_length(),data,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    glGenVertexArrays(1, &vao_handle);
    glBindVertexArray(vao_handle);
    glBindBuffer(GL_ARRAY_BUFFER,buffer_handle);
    GLuint stride = 0;
    for(GLuint i = 0; i < attribute_arrays.size(); i++) {
      stride += attribute_arrays[i]->get_vertex_length_in_bytes();
    }
    GLuint offset = 0;
    for(GLuint i = 0; i < attribute_arrays.size(); i++) {
      glEnableVertexAttribArray(i);
      glVertexAttribPointer(i,attribute_arrays[i]->get_nbr_elements_per_vertex(),GL_FLOAT,attribute_arrays[i]->should_normalize(),stride,reinterpret_cast<GLvoid *>(offset));
      offset += attribute_arrays[i]->get_vertex_length_in_bytes();
    }
    glBindVertexArray(0);

    free(data);
  } else {
    assert(0);
    //TODO
  }
}

GLfloat * Mesh::create_buffer_data(GLboolean interleaved) {
  GLuint buffer_length = calculate_buffer_length();
  GLfloat * data = (GLfloat *)malloc(buffer_length); // This is very bad, but...
  if(!data) {
    std::cerr << "Not enough ram for buffer data.";
    return 0;
  }
  GLuint position_in_buffer = 0;

  if(attribute_arrays.size() > 0) {
    GLuint size = attribute_arrays[0]->get_nbr_vertices();
    GLuint temp_size = 0;
    //Check that there is equal amount of vertices in each attribute array.
    for(GLuint i = 1; i < attribute_arrays.size();i++) {
      temp_size = attribute_arrays[i]->get_nbr_vertices();
      if(temp_size != size) {
	std::cerr << "Attribute arrays have different lengths. Data not saved into memory." << std::endl;
	return 0;
      }
    }
    for(GLuint i = 0; i < attribute_arrays[0]->get_nbr_vertices();i++) {
      for(GLuint j = 0; j < attribute_arrays.size(); j++) {
	memcpy((GLbyte*)data + position_in_buffer,(GLbyte *)attribute_arrays[j]->get_data() + attribute_arrays[j]->get_vertex_length_in_bytes()*i,attribute_arrays[j]->get_vertex_length_in_bytes());
	position_in_buffer += attribute_arrays[j]->get_vertex_length_in_bytes();
      }
    }
  }
#ifdef DEBUG
  print_contex((GLfloat *)data,calculate_buffer_length()/4);
#endif
  return data;
}

GLuint Mesh::calculate_buffer_length() {
  GLuint length = 0;
  for(GLuint i = 0; i < attribute_arrays.size(); i++) {
    length += attribute_arrays[i]->get_data_length_in_bytes();
  }
  return length;
}

void Mesh::set_drawing_mode(DrawingMode drawing_mode) {
  this->drawing_mode = drawing_mode;
}

/** 
 * Appends data to the vertex attribute array at specified index. Note that the operation is not typesafe. Providing data with wrong data will lead to undefined behaviour.
 * 
 * @param nbr_vertices Number of new vertices.
 * @param data Pointer to the beginning of the data.
 * @param index Index of the Vertex attribute array.
 */

GLuint Mesh::append_data(GLuint nbr_vertices, GLfloat * data, GLuint index) {
  if(index < attribute_arrays.size()) {
    return attribute_arrays[index]->append_data(nbr_vertices,data);
  } else {
    return 0;
  }
}

void Mesh::draw(DrawingMode drawing_mode_param) {
  DrawingMode local_mode;
  if(drawing_mode_param == NOTSET) {
    local_mode = this->drawing_mode;
  } else {
    local_mode = drawing_mode_param;
  }
  //std::cout << local_mode << drawing_mode_param << NOTSET << std::endl;
  switch(local_mode) {
  case ARRAY:
    glBindVertexArray(vao_handle);
    glDrawArrays(polygon_type,0,attribute_arrays[0]->get_nbr_vertices());
    glBindVertexArray(0);
    break;
  case INDEXED:
    glBindVertexArray(vao_handle);
    glDrawElements(polygon_type,index_array->get_nbr_vertices(),GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
    break;
  case ADJACENCY:
    glBindVertexArray(vao_handle);
    glDrawElements(GL_TRIANGLES_ADJACENCY,index_array->get_nbr_vertices()*2,GL_UNSIGNED_INT,reinterpret_cast<GLvoid *>(index_array->get_buffer_size()));
    glBindVertexArray(0);

    break;
  default:
    assert(0);
    break;
  }
  GLenum err;
  while((err = glGetError()) != GL_NO_ERROR)
    {
      std::cerr << "OpenGL error: " << err << std::endl;
    }
}

void Mesh::print_contex(const GLfloat * input,GLuint nbr_elements) const {
  for(GLuint i = 0; i < nbr_elements; i++) {
    std::cout << *(input + i) << ", ";
    if(i%7 == 6) {
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;
} 

/*
int main() {
  Mesh * mesh = new Mesh();
  GLfloat data[] = {1.5,3.5,5.5,7.5,9.5,11.5};
  GLint data2[] = {2,4,6,8,10,12};
  mesh->add_attribute_array(FLOAT,3,2,(GLvoid *) data);
  mesh->add_attribute_array(INT,3,2,(GLvoid *) data2);
  mesh->save_to_memory();
}
*/

/** 
 * 
 * 
 * 
 * @return 
 */

GLint Mesh::convert_to_triangles() {
  //TODO does nothing yet.
  if(index_array) {
    GLuint nbr_vertices_new;
    GLuint * triangle_index_array;
    GLuint * old_data = index_array->get_data();
    
    switch(polygon_type) {
    case QUAD:
      nbr_vertices_new = index_array->get_nbr_vertices()/4*6; // two triangles from one quad.
      triangle_index_array = (GLuint *)malloc(sizeof(GLuint)*nbr_vertices_new);
      if(!triangle_index_array) {
	std::cerr << "Out of memory." << std::endl;
	return 3;
      }
      for(GLint i = 0; i < index_array->get_nbr_vertices()/4; i++) { // Loop over faces
	triangle_index_array[i*6] = old_data[i*4];
	triangle_index_array[i*6+1] = old_data[i*4+1];
	triangle_index_array[i*6+2] = old_data[i*4+2];
	triangle_index_array[i*6+3] = old_data[i*4+2];
	triangle_index_array[i*6+4] = old_data[i*4+3];
	triangle_index_array[i*6+5] = old_data[i*4];
      }
      set_index_array(nbr_vertices_new,triangle_index_array);
      set_polygon_type(TRIANGLE);
      return 0;
    case QUAD_STRIP:
      return 1;
    default:
      return 1;
    }
  } else {
    return 2;
  }
}

static GLuint get_opposite_index(const GLuint * face, const Edge e) {
  for(GLuint i = 0; i < 3; i++) {
    GLuint index = face[i];
    if(index != e.get_vertex1() && index != e.get_vertex2()) {
      return index;
    }
  }
  assert(0);
  return 0;
}

//Mostly copied from http://ogldev.atspace.co.uk/www/tutorial39/tutorial39.html

GLuint Mesh::create_adjacency_data() {
  if(!index_array) {
    return 1; //Index array not set
  }
  if(polygon_type != TRIANGLE) {
    return 1;
  }
  if(adjacency_array) {
    free(adjacency_array);
    adjacency_array = 0;
  }
  adjacency_array = (GLuint *)malloc(sizeof(GLuint)*index_array->get_nbr_vertices()*2); // space for adjacency data and actual data
  
  GLuint * index_data = index_array->get_data();
  std::map<Edge,Neighbours,ComparaEdges> index_map;
  for(GLuint i = 0; i < index_array->get_nbr_vertices()/3;i++) { // Loop over all the faces (divided by three because triangles need 3 indices).
    Edge e1(index_data[i*3],index_data[i*3+1]);
    Edge e2(index_data[i*3+1],index_data[i*3+2]);
    Edge e3(index_data[i*3+2],index_data[i*3]);

    index_map[e1].add_neighbour(i);
    index_map[e2].add_neighbour(i);
    index_map[e3].add_neighbour(i); // If the mesh is bad may segfault here.
  }
  for(GLuint i = 0; i < index_array->get_nbr_vertices()/3;i++) {
    
    for(GLuint j = 0; j < 3; j++) { //Again 3 for triangle.
      Edge e(index_data[i*3+j],index_data[i*3 + (j+1) % 3]); // {{0,1},{1,2},{2,0}}
      assert(index_map.find(e) != index_map.end()); //Edge (key) not found in the index_map.
      Neighbours n = index_map[e];
      GLuint other_face = n.get_other(i);
      
      if(other_face == -1) { // Not set.
	other_face = 0;
      }
      const GLuint opposite_index = get_opposite_index(index_data + other_face*3,e);
      adjacency_array[(i*3 + j)*2 + 0] = index_data[i*3 + j];
      adjacency_array[(i*3 + j)*2 + 1] = opposite_index;
    }
  }
  //this->adjacency_array = adjacency_array;
  return 0;
}



/**
   Moves every element of the attribute array specified byt the attribute_array_index by the amount given by displacement array.
 **/

GLboolean Mesh::displace_mesh(GLuint attribute_array_index, GLuint elements, const GLfloat * displacement) {
  if(attribute_arrays.size() < attribute_array_index) {
    return false; // Array index is too large
  }
  AttributeArray * target_array = attribute_arrays[attribute_array_index];
  if(target_array->get_nbr_elements_per_vertex() != elements) {
    return false;
  }
  GLuint nbr_vertices = target_array->get_nbr_vertices();
  GLfloat * data = target_array->get_data();
  for(GLuint i = 0; i < nbr_vertices; i++) {
    for(GLuint j = 0; j < elements; j++) {
      data[i*elements + j] = data[i*elements+j] - displacement[j];
    }
  }
  if(interleaved) { //TODO IF INTERLEAVED
    glBindBuffer(GL_ARRAY_BUFFER,buffer_handle);
    GLfloat * buffer_map = reinterpret_cast<GLfloat *>(glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY));
    if(buffer_map == NULL) {
      // Mapping failed
      return false;
    }
    GLuint stride = 0;
    GLuint offset = 0;
    for(GLuint i = 0; i < attribute_arrays.size(); i++) {
      if(i == attribute_array_index) {
	offset = stride;
      }
      stride += attribute_arrays[i]->get_nbr_elements_per_vertex(); // Expect that the data is floats.
    }
    for(GLuint i = 0; i < nbr_vertices; i++) {
      for(GLuint j = 0; j < elements; j++) {
	buffer_map[i*stride + offset + j] = data[i*elements + j];
      }
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER,0);
  } else {
    //TODO 
    assert(0);
  }
}

/**
   Rotates every vertex of the mesh by given rotation matrix. Works only for vertices with 3 or 4 elements. The fourth element is left intact.
 **/

GLboolean Mesh::rotate_mesh(GLuint attribute_array_index, const glm::mat3 & rotation_matrix) {
  if(attribute_arrays.size() < attribute_array_index) {
    assert(0);
    return false; // Array index is too large
  }
  AttributeArray * target_array = attribute_arrays[attribute_array_index];
  GLuint elements = target_array->get_nbr_elements_per_vertex();
  if(elements != 3 && elements != 4 ) {
    return false;
  }
  GLuint nbr_vertices = target_array->get_nbr_vertices();
  GLfloat * data = target_array->get_data();
  for(GLuint i = 0; i < nbr_vertices; i++) {
    glm::vec3 point;
    for(GLuint j = 0; j < 3; j++) { // Loop only up to three, fourth element is not used for rotation
      point[j] = data[i*elements+j];
    }
    glm::vec3 rotated_point = rotation_matrix*point;
    for(GLuint j = 0; j < 3; j++) {
      data[i*elements + j] = rotated_point[j];
    }
  }

  if(interleaved) { //TODO IF NOT INTERLEAVED
    glBindBuffer(GL_ARRAY_BUFFER,buffer_handle);
    GLfloat * buffer_map = reinterpret_cast<GLfloat *>(glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY));
    if(buffer_map == NULL) {
      assert(0);
      // Mapping failed
      return false;
    }
    GLuint stride = 0;
    GLuint offset = 0;
    for(GLuint i = 0; i < attribute_arrays.size(); i++) {
      if(i == attribute_array_index) {
	offset = stride;
      }
      stride += attribute_arrays[i]->get_nbr_elements_per_vertex(); // Expect that the data is floats.
    }
    for(GLuint i = 0; i < nbr_vertices; i++) {
      for(GLuint j = 0; j < 3; j++) {
	buffer_map[i*stride + offset + j] = data[i*elements + j];
      }
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER,0);
  } else {
    assert(0);
    //TODO
  }
}

void Mesh::get_center_of_mass(GLuint attribute_array_index,glm::vec3 & center_of_mass, GLfloat & mass) {
  assert(polygon_type == TRIANGLE); //TODO other cases
  if(!(attribute_array_index < attribute_arrays.size())) {
    assert(0); // Array index incorrect
    //return glm::vec3(0,0,0);
    
  }
  AttributeArray * attribute_array = attribute_arrays[attribute_array_index];
  if(attribute_array->get_nbr_elements_per_vertex() != 3 && attribute_array->get_nbr_elements_per_vertex() != 4) {
    assert(0);  // Incorrect number of elements per vertex
    //return glm::vec3(0,0,0);
  }
  GLfloat volume = 0;
  glm::vec3 intx2n = glm::vec3(0,0,0);
  GLuint nbr_elements = attribute_array->get_nbr_elements_per_vertex();
  GLfloat * data = attribute_array->get_data();
  if(index_array != 0) { // Triangles from indices
    GLuint * id = index_array->get_data();
    for(GLuint i = 0; i < index_array->get_nbr_vertices()/3; i++) {
      glm::vec3 a = glm::vec3(data[id[i*3]*nbr_elements+0],data[id[i*3]*nbr_elements+1],data[id[i*3]*nbr_elements+2]);
      glm::vec3 b = glm::vec3(data[id[i*3+1]*nbr_elements+0],data[id[i*3+1]*nbr_elements+1],data[id[i*3+1]*nbr_elements+2]);
      glm::vec3 c = glm::vec3(data[id[i*3+2]*nbr_elements+0],data[id[i*3+2]*nbr_elements+1],data[id[i*3+2]*nbr_elements+2]);
      glm::vec3 normal = glm::cross(c-a,b-a);
      volume += glm::dot(a,normal);
      intx2n += 1/24.0f*normal*((a + b)*(a + b) + (b + c)*(b + c) + (c + a)*(c + a));
    }
    //volume = std::abs(volume)/6;
    volume = volume/6;
    //std::cout << "Volume : " << volume << std::endl;
    glm::vec3 com = intx2n/(2*volume);
    //std::cout << "com: " << com.x << ", " << com.y <<", " << com.z << std::endl; // THIS RETURNS THE OPPOSITE OF COM
    //return com;
    center_of_mass = -com;
    mass = -volume;
  } else { //ARRAY
    assert(0); //TODO
  }
}

// THIS IS NO CALCULATED BY get_center_of_mass(..)
/*
GLfloat Mesh::get_volume(GLuint attribute_array_index) {
  assert(polygon_type == TRIANGLE); //TODO other cases
  if(!(attribute_array_index < attribute_arrays.size())) {
    assert(0); // Array index incorrect
  }
  AttributeArray * attribute_array = attribute_arrays[attribute_array_index];
  if(attribute_array->get_nbr_elements_per_vertex() != 3 && attribute_array->get_nbr_elements_per_vertex() != 4) {
    assert(0);  // Incorrect number of elements per vertex
  }
  if(attribute_array->get_nbr_elements_per_vertex() != 3 && attribute_array->get_nbr_elements_per_vertex() != 4) {
    assert(0);  // Incorrect number of elements per vertex
  }
  GLfloat volume = 0;
  GLuint nbr_elements = attribute_array->get_nbr_elements_per_vertex();
  GLfloat * data = attribute_array->get_data();
  if(index_array != 0) { // Triangles from indices
    GLuint * id = index_array->get_data();
    for(GLuint i = 0; i < index_array->get_nbr_vertices()/3; i++) {
      glm::vec3 a = glm::vec3(data[id[i*3]*nbr_elements+0],data[id[i*3]*nbr_elements+1],data[id[i*3]*nbr_elements+2]);
      glm::vec3 b = glm::vec3(data[id[i*3+1]*nbr_elements+0],data[id[i*3+1]*nbr_elements+1],data[id[i*3+1]*nbr_elements+2]);
      glm::vec3 c = glm::vec3(data[id[i*3+2]*nbr_elements+0],data[id[i*3+2]*nbr_elements+1],data[id[i*3+2]*nbr_elements+2]);
      glm::vec3 normal = glm::cross(c-a,b-a);
      volume += glm::dot(a,normal);
    }
  }
  volume = volume/6;
  return volume;
}
*/
/**
   Calculates the covariance matrix of a triangular mesh. Can be applied in calculating the moment of intertia using J = I*Tr(C) - C. However, it is not used at the moment.
**/

glm::mat3 Mesh::get_covariance_matrix(GLuint attribute_array_index) {
    assert(polygon_type == TRIANGLE); //TODO other cases
  if(!(attribute_array_index < attribute_arrays.size())) {
    assert(0); // Array index incorrect
  }
  AttributeArray * attribute_array = attribute_arrays[attribute_array_index];
  if(attribute_array->get_nbr_elements_per_vertex() != 3 && attribute_array->get_nbr_elements_per_vertex() != 4) {
    assert(0);  // Incorrect number of elements per vertex
  }
  
  GLuint nbr_elements = attribute_array->get_nbr_elements_per_vertex();
  GLfloat * data = attribute_array->get_data();
  glm::mat3 C = glm::mat3(0.0);
  if(index_array != 0) { // Triangles from indices
    GLuint * id = index_array->get_data();
    for(GLuint i = 0; i < index_array->get_nbr_vertices()/3; i++) {
      glm::vec3 a = glm::vec3(data[id[i*3]*nbr_elements+0],data[id[i*3]*nbr_elements+1],data[id[i*3]*nbr_elements+2]);
      glm::vec3 b = glm::vec3(data[id[i*3+1]*nbr_elements+0],data[id[i*3+1]*nbr_elements+1],data[id[i*3+1]*nbr_elements+2]);
      glm::vec3 c = glm::vec3(data[id[i*3+2]*nbr_elements+0],data[id[i*3+2]*nbr_elements+1],data[id[i*3+2]*nbr_elements+2]);
      /*GLfloat Aa[9] = {a.x,a.y,a.z,
		       b.x,b.y,b.z,
		       c.x,c.y,c.z};*/
      GLfloat Aa[9] = {b.x,b.y,b.z,
		       a.x,a.y,a.z,
		       c.x,c.y,c.z};
      /*GLfloat Aa[9] = {a.x,b.x,c.x,
		       a.y,b.y,c.y,
		       a.z,b.z,c.z};*/
      GLfloat Cca[9] = {1.0/60,1.0/120,1.0/120,
			1.0/120,1.0/60,1.0/120,
			1.0/120,1.0/120,1.0/60};
      glm::mat3 Cc = glm::make_mat3(Cca);
      glm::mat3 A = glm::make_mat3(Aa);
      C += glm::determinant(A)*A*Cc*glm::transpose(A);
      //std::cout << glm::determinant(A) << std::endl;
    }
  } else {
    assert(0); // No index array set
  }
  return C;
}

/**
   Returns the points of the mesh as a vector. Only triangular mesh is handled at the moment.
 **/

void Mesh::get_points(std::vector<glm::vec4> & output,GLuint attribute_array_index) {
  assert(polygon_type == TRIANGLE); //TODO other cases
  if(!(attribute_array_index < attribute_arrays.size())) {
    assert(0);
  }
  AttributeArray * attribute_array = attribute_arrays[attribute_array_index];
  if(attribute_array->get_nbr_elements_per_vertex() != 3 && attribute_array->get_nbr_elements_per_vertex() != 4) {
    assert(0);  // Incorrect number of elements per vertex
  }
  output.reserve(output.size() + attribute_array->get_nbr_vertices());
  GLuint nbr_elements = attribute_array->get_nbr_elements_per_vertex();
  GLfloat * data = attribute_array->get_data();
  for(GLuint i = 0; i < attribute_array->get_nbr_vertices(); i++) {
    if(nbr_elements == 3) {
      output.push_back(glm::vec4(data[i*nbr_elements + 0],data[i*nbr_elements + 1],data[i*nbr_elements + 2],1.0));
    } else {
      output.push_back(glm::vec4(data[i*nbr_elements + 0],data[i*nbr_elements + 1],data[i*nbr_elements + 2],data[i*nbr_elements + 3]));
    }
  }
}

/**
   Calculates the moment of inertia of a triangular mesh. The center of mass of the mesh is expected to locate in the origin. NOTE! I think that this actually calculates the moment of inertia around the origin. It does not matter where the center of mass is.
 **/

glm::mat3 Mesh::get_moment_of_inertia(GLuint attribute_array_index) {//, const glm::vec3 & rp) {
  assert(polygon_type == TRIANGLE); //TODO other cases
  if(!(attribute_array_index < attribute_arrays.size())) {
    assert(0); // Array index incorrect
  }
  AttributeArray * attribute_array = attribute_arrays[attribute_array_index];
  if(attribute_array->get_nbr_elements_per_vertex() != 3 && attribute_array->get_nbr_elements_per_vertex() != 4) {
    assert(0);  // Incorrect number of elements per vertex
  }
  
  GLuint nbr_elements = attribute_array->get_nbr_elements_per_vertex();
  GLfloat * data = attribute_array->get_data();
  glm::mat3 C = glm::mat3(0.0);
  if(index_array != 0) { // Triangles from indices
    GLuint * id = index_array->get_data();
    for(GLuint i = 0; i < index_array->get_nbr_vertices()/3; i++) {
      glm::vec3 a = glm::vec3(data[id[i*3]*nbr_elements+0],data[id[i*3]*nbr_elements+1],data[id[i*3]*nbr_elements+2]);
      glm::vec3 b = glm::vec3(data[id[i*3+1]*nbr_elements+0],data[id[i*3+1]*nbr_elements+1],data[id[i*3+1]*nbr_elements+2]);
      glm::vec3 c = glm::vec3(data[id[i*3+2]*nbr_elements+0],data[id[i*3+2]*nbr_elements+1],data[id[i*3+2]*nbr_elements+2]);
      /*GLfloat Aa[9] = {a.x,a.y,a.z,
		       b.x,b.y,b.z,
		       c.x,c.y,c.z};*/
      GLfloat Aa[9] = {b.x,b.y,b.z,
		       a.x,a.y,a.z,
		       c.x,c.y,c.z};
      /*GLfloat Aa[9] = {a.x,b.x,c.x,
		       a.y,b.y,c.y,
		       a.z,b.z,c.z};*/
      GLfloat Cca[9] = {1.0/60,1.0/120,1.0/120,
			1.0/120,1.0/60,1.0/120,
			1.0/120,1.0/120,1.0/60};
      glm::mat3 Cc = glm::make_mat3(Cca);
      glm::mat3 A = glm::make_mat3(Aa);
      C += glm::determinant(A)*A*Cc*glm::transpose(A);
      //std::cout << glm::determinant(A) << std::endl;
    }
  } else {
    assert(0); // No index array set
  }
  // Transformation to global center of mass
  /*GLfloat dxdxt[9] = {rp.x*rp.x,rp.x*rp.y,rp.x*rp.z,
		      rp.y*rp.x,rp.y*rp.x,rp.y*rp.z,
		      rp.z*rp.z,rp.z*rp.y,rp.z*rp.z};
  GLfloat volume = calculate_volume(attribute_array_index);
  C += std::abs(volume)*glm::make_mat3(dxdxt);*/
  glm::mat3 I = glm::mat3(1.0)*(C[0][0] + C[1][1] + C[2][2]) - C;
  /*  
  for(GLuint i = 0; i < 3; i++) {
    for(GLuint j = 0; j < 3; j++) {
      std::cout << C[i][j] << ",\t";
    }
    std::cout << std::endl;
  }
  for(GLuint i = 0; i < 3; i++) {
    for(GLuint j = 0; j < 3; j++) {
      std::cout << I[i][j] << ",\t";
    }
    std::cout << std::endl;
  }
  */
  return -I;
}

GLboolean Mesh::get_index_data(std::vector<GLuint> & index_data) {
  if(index_array == 0) {
    return GL_FALSE;
  }
  GLuint * index_data_array = this->index_array->get_data();
  GLuint nbr_vertices = this->index_array->get_nbr_vertices();
  index_data.reserve(nbr_vertices);
  for(GLuint ii=0; ii<nbr_vertices;ii++) {
    index_data.push_back(index_data_array[ii]);
  }
  return GL_TRUE;
}


void Mesh::get_random_points_inside_mesh(GLuint nbr_points,GLuint target_attribute_array,std::vector<glm::vec4> & output,GLuint seed,GLboolean is_convex) {
  
  if(!(target_attribute_array < attribute_arrays.size())) {
    assert(0);
  }

  if(polygon_type == TRIANGLE) {
    // First calculate the bounding aabb
    std::vector<glm::vec4> mesh_points;
    get_points(mesh_points,0);
    BoundingAABB * aabb = BGAlgorithm::boundingAABB_from_points(mesh_points);
    /*
    std::cout << aabb->get_trans_min()[0] << ", " << aabb->get_trans_min()[1] << ", " << aabb->get_trans_min()[2] << std::endl;
    std::cout << aabb->get_trans_max()[0] <<  ", " <<aabb->get_trans_max()[1] << ", " << aabb->get_trans_max()[2] << std::endl;
    */
    std::mt19937 gen(seed);
    std::uniform_real_distribution<GLfloat> x_dist(aabb->get_trans_min()[0],aabb->get_trans_max()[0]);
    std::uniform_real_distribution<GLfloat> y_dist(aabb->get_trans_min()[1],aabb->get_trans_max()[1]);
    std::uniform_real_distribution<GLfloat> z_dist(aabb->get_trans_min()[2],aabb->get_trans_max()[2]);
    GLuint nbr_points_acquired = 0;
    std::vector<GLuint> index_data;
    GLboolean index_array_exists = get_index_data(index_data);
    if(!index_array_exists) {
      assert(0); // Only works for indexed data atm
    }
    GLuint nbr_triangles = index_data.size()/3; //Divided by three, because triangle has th
    GLuint failure_counter = 0;
    const GLuint failure_limit = 1000;
    while (nbr_points_acquired < nbr_points) {
    stop:
      if(failure_counter >= failure_limit) {
	return;
      }
      GLfloat x_rnd = x_dist(gen);
      GLfloat y_rnd = y_dist(gen);
      GLfloat z_rnd = z_dist(gen);
      glm::vec3 rnd_point = glm::vec3(x_rnd,y_rnd,z_rnd);
      // THIS WORKS FOR CONVEX OBJECTS
      for(GLuint ii=0; ii<nbr_triangles;ii++) {
	// Move these outside of loop
	glm::vec3 v0 = glm::vec3(mesh_points[index_data[ii*3]]);
	glm::vec3 v1 = glm::vec3(mesh_points[index_data[ii*3+1]]);
	glm::vec3 v2 = glm::vec3(mesh_points[index_data[ii*3+2]]);
	glm::vec3 norm = glm::cross(v1-v0,v2-v0);
	if(glm::dot(norm,norm) > 0.0001f) { // Check that norm exists
	  
	  if(glm::dot(v0-rnd_point,norm) < 0) { // Point is outised
	    if(is_convex) {
	      //debug_target_points.push_back(glm::vec4(v0,1.0));
	      failure_counter++;
	      goto stop;
	    } else { // not convex
	      GLuint nbr_layers = 0;
	      glm::vec3 target_point_for_ray = v0 + 0.333f*(v1-v0) + 0.333f*(v2-v0); // middle of the triangle.
	      glm::vec3 d = target_point_for_ray-rnd_point;
	      for(GLuint jj=0; jj<nbr_triangles;jj++) {
		if(jj==ii) { // We know that the ray intersects the original triangle
		  continue;
		}
		glm::vec3 v0_j = glm::vec3(mesh_points[index_data[jj*3]]);
		glm::vec3 v1_j = glm::vec3(mesh_points[index_data[jj*3+1]]);
		glm::vec3 v2_j = glm::vec3(mesh_points[index_data[jj*3+2]]);
		//if(glm::distance2(v0,v0_j) > 0.001f && glm::distance2(v0,v1_j) > 0.001f && glm::distance2(v0,v2_j) > 0.001f) {
		// Somehow deal with hitting the corner or the edge.
		  GLboolean ray_intersects = CollisionDetectionAlgorithms::ray_triangle_intersection(v0_j,v1_j,v2_j,rnd_point,d);
		  if(ray_intersects) {
		    nbr_layers++;
		    /*
		      glm::vec3 norm_j = glm::cross(v1_j-v0_j,v2_j-v0_j);
		      if(glm::dot(norm,norm) > 0.0001f) { // Check that norm exists
		      if(glm::dot(v0_j-rnd_point,norm_j) < 0) {
		      nbr_layers++;
		      }
		      }
		    */
		  }
		  //}
	      }
	      std::cout << nbr_layers << std::endl;
	      if(nbr_layers % 2 == 1) {
		failure_counter++;
		//std::cout << "Failure!" << std::endl;
		//debug_points.push_back(glm::vec4(rnd_point,1));
		//point_accepted.push_back(GL_FALSE);
		//debug_target_points.push_back(glm::vec4(target_point_for_ray,1.0));
		goto stop;
	      } else { // The point is inside!
		// Should do nothing
		//debug_points.push_back(glm::vec4(rnd_point,1));
		//point_accepted.push_back(GL_TRUE);
		//debug_target_points.push_back(glm::vec4(v0,1));
		//goto success;
		
	      }
	    }
	  }
	  /*
	    if(glm::dot(v1-rnd_point,norm) < 0) {
	    failure_counter++;
	    goto stop;
	    }
	    if(glm::dot(v2-rnd_point,norm) < 0) {
	    failure_counter++;
	    goto stop;
	    }
	  */
	}
      }
      /*
      std::cout << "Failure!" << std::endl;
      failure_counter++;
      goto stop;
      */
      //success:
      output.push_back(glm::vec4(rnd_point,1));
      //debug_points.push_back(glm::vec4(rnd_point,1));
      //debug_target_points.push_back(glm::vec4(0,0,0,1));
      //point_accepted.push_back(GL_TRUE);
      //std::cout << "Point found!" << std::endl;
      failure_counter = 0;
      //std::cout << "Random number from mesha: x=" << x_rnd << " y=" << y_rnd << " z=" << z_rnd << std::endl;
      nbr_points_acquired++;
    }
  } else {
    assert(0); // Only triangle type works
  }
}

void Mesh::insert_adjacency_array(std::vector<GLuint> & adjacency_data) {
  if(adjacency_array != NULL) {
    free(adjacency_array);    
  }
  adjacency_array = new GLuint[adjacency_data.size()];
  for(size_t ii=0; ii<adjacency_data.size(); ii++) {
    adjacency_array[ii] = adjacency_data[ii];
  }
}
