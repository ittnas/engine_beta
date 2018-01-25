#include "movable.hpp"

Movable::Movable() : Object() {
  this->model_buffer = new ModelBuffer();
  if(this->model_buffer == NULL) {
    assert(0); //Out of memory! :(
  }
  this->add_buffer(this->model_buffer);
}

void Movable::set_center_of_mass(const glm::vec3 & com) {
  model_buffer->set_center_of_mass(com);
}

glm::vec3 Movable::get_center_of_mass() const {
  glm::vec3 com = model_buffer->get_center_of_mass();
  return com;
}
/** 
 * Returns the points in the Mesh of this Object and all the child Objects, which implement Movable and pass the physics flag test. It is expected that all the meshes constitute of coordinates with 3 or 4 dimensions (x,y,z[,w]).
 * 
 * @param attribute_array_index index of the target attribute array
 * @param physics_flag physics flag which is used to do the comparison
 * @param comp comparator enum
 * 
 * @return vector containing the points of all the child objects, which pass the criteria
 */

void Movable::get_transformed_mesh_points(std::vector<glm::vec4> & output,GLuint attribute_array_index, GLuint physics_flag,Comparator comp) {
  if(skip_physics(physics_flag,comp)) {
    return;
  }
  if(get_shape() != 0) {
    get_shape()->get_mesh()->get_points(output,attribute_array_index);
  }
  for(auto child : get_children()) {
    Movable * mv_child = dynamic_cast<Movable *>(child);
    if(mv_child != NULL) {
      mv_child->get_transformed_mesh_points(output,attribute_array_index,physics_flag,comp);
    }
    //output.insert(output.end(), childs_points.begin(), childs_points.end());
  }
  // TODO might be good idea not to include this. The position of the rigid body might change, which should not affect the position of the mesh of the bounding geometry. Note, this version appears to be working well.
  for(GLuint i = 0; i < output.size();i++) {
    output[i] = get_model_matrix()*output[i];
  }
  
}
