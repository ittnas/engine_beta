#include <assert.h>
#include <limits> 
#include <glm/glm.hpp>
#include <cstring>
#include <GL/glew.h>
#include <iostream>

#include "rigid_body.hpp"
#include "simple_collision_detector.hpp"

Aabb * SimpleCollisionDetector::generate_Aabb(Mesh * mesh, GLuint position_array_index) {
  AttributeArray * position_array = mesh->get_attribute_array(position_array_index);

  assert(position_array->get_nbr_elements_per_vertex() == 4);
  GLfloat * data = position_array->get_data();
  GLfloat max[3] = {-std::numeric_limits<GLfloat>::max(),-std::numeric_limits<GLfloat>::max(),-std::numeric_limits<GLfloat>::max()};
  GLfloat min[3] = {std::numeric_limits<GLfloat>::max(),std::numeric_limits<GLfloat>::max(),std::numeric_limits<GLfloat>::max()};
  for(GLuint i = 0; i < position_array->get_nbr_vertices(); i++) {
    for(GLuint j = 0; j < 3; j++) {
      if(data[i*4 + j] > max[j]) {
	max[j] = data[i*4+j];
      }
      if(data[i*4 + j] < min[j]) {
	min[j] = data[i*4+j];
      }
    }
  }
  return new Aabb(min,max);
}

void SimpleCollisionDetector::multiply_Aabb_with_matrix(Aabb * source, Aabb * destination, const glm::mat4 & model_matrix) {
  glm::vec4 new_vertices[8];
  glm::vec4 * old_vertices = source->get_vertices();
  for(GLuint i = 0; i < 8; i++) {
    new_vertices[i] = model_matrix * old_vertices[i];
  }
  GLfloat min[3] = {std::numeric_limits<GLfloat>::max(),std::numeric_limits<GLfloat>::max(),std::numeric_limits<GLfloat>::max()};
  GLfloat max[3] = {-std::numeric_limits<GLfloat>::max(),-std::numeric_limits<GLfloat>::max(),-std::numeric_limits<GLfloat>::max()};
  
  for(GLuint i = 0; i < 8; i++) {
    if(new_vertices[i].x < min[0]) {
      min[0] = new_vertices[i].x;
    }
    if(new_vertices[i].y < min[1]) {
      min[1] = new_vertices[i].y;
    }
    if(new_vertices[i].z < min[2]) {
      min[2] = new_vertices[i].z;
    }
    if(new_vertices[i].x > max[0]) {
      max[0] = new_vertices[i].x;
    }
    if(new_vertices[i].y > max[1]) {
      max[1] = new_vertices[i].y;
    }
    if(new_vertices[i].z > max[2]) {
      max[2] = new_vertices[i].z;
    }
  }
  destination->set_min_max(min,max);
}

void SimpleCollisionDetector::add_object(Object * object) {
  UniformMaterial * mv = dynamic_cast<UniformMaterial *>(object);
  //RigidBody * mv = dynamic_cast<RigidBody *>(object);
  if(mv != 0 && object->get_shape() != 0) {
    objects.push_back(mv);
    Mesh * mesh =  mv->get_shape()->get_mesh();
    Aabbs_from_mesh.push_back(generate_Aabb(mesh,0));
    GLfloat min[3];
    GLfloat max[3];
    Aabbs.push_back(new Aabb(min,max));
  }
}

void SimpleCollisionDetector::apply_transformations() {
  for(GLuint i = 0; i < objects.size(); i++) {
    multiply_Aabb_with_matrix(Aabbs_from_mesh[i],Aabbs[i],objects[i]->get_model_matrix());
  }
}

void SimpleCollisionDetector::detect_collisions() {
  for(GLuint i = 0; i < Aabbs.size(); i++) {
    for(GLuint j = i+1; j < Aabbs.size(); j++) {
      if(collision_takes_place(Aabbs[i],Aabbs[j])) {
	objects[i]->set_ambient_color(glm::vec4(0.2,0.0,0.0,1.0));
	  objects[j]->set_ambient_color(glm::vec4(0.2,0.0,0.0,1.0));
	/*RigidBody * rb1 = (RigidBody*)(objects[i]);
	rb1->apply_impulse(glm::vec3(1,0,0),glm::vec3(1.0,3.0,0));
	RigidBody * rb2 = (RigidBody*)(objects[j]);
	rb2->apply_impulse(glm::vec3(-1,0,0),glm::vec3(1,3.0,0));*/
      } else {
	objects[i]->set_ambient_color(glm::vec4(0.0,0.2,0.0,1.0));
	  objects[j]->set_ambient_color(glm::vec4(0.0,0.2,0.0,1.0));
      }
    }
  }
}

GLboolean SimpleCollisionDetector::collision_takes_place(Aabb * obj1, Aabb* obj2) {
  //std::cout << "obj1 max: " << obj1->get_max()[0] << "," << obj1->get_max()[1] << "," << obj1->get_max()[2] << " min: " << obj1->get_min()[0] << "," << obj1->get_min()[1] << "," << obj1->get_min()[2] << std::endl;
  //std::cout << "obj2 max: " << obj2->get_max()[0] << "," << obj2->get_max()[1] << "," << obj2->get_max()[2] << " min: " << obj2->get_min()[0] << "," << obj2->get_min()[1] << "," << obj2->get_min()[2] << std::endl;
  for(GLuint i = 0; i < 3; i++) {
    if(obj1->get_max()[i] < obj2->get_min()[i] || obj1->get_min()[i] > obj2->get_max()[i]) {
      return false;
    }
  }
  return true;
}
