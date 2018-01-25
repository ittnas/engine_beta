#include <memory>
#include "simple_collision_hierarchy.hpp"

void SimpleCollisionHierarchy::get_intersecting_pairs(std::vector<std::unique_ptr<CollisionPair>> & input) {
  for(GLuint i = 0; i < collision_hierarchy_nodes.size(); i++) {
    for(GLuint j = i+1;j < collision_hierarchy_nodes.size(); j++) {
      //TODO check if bounding geometries exist
      GLboolean intersect_true;
      GLfloat depth_of_intersection;
      glm::vec3 point_of_intersection;
      glm::vec3 normal_of_intersection;
      //std::tie(intersect_true,point_of_intersection,normal_of_intersection,depth_of_intersection) = collision_hierarchy_nodes[i]->get_bounding_geometry()->check_intersection(*(collision_hierarchy_nodes[j]->get_bounding_geometry()),GL_TRUE);
      //std::cout << normal_of_intersection.x << ". From simple_collision_hierarchy.cpp/get_intersecting_pairs" << std::endl;
      intersect_true = collision_hierarchy_nodes[i]->get_bounding_geometry()->check_intersection(*(collision_hierarchy_nodes[j]->get_bounding_geometry()),GL_TRUE,&point_of_intersection,&normal_of_intersection,&depth_of_intersection);

      if(intersect_true) {
	CollisionObject * co1 = collision_hierarchy_nodes[i]->get_collision_object();
	CollisionObject * co2 = collision_hierarchy_nodes[j]->get_collision_object();
	//std::cout << co1 << std::endl;
	//std::cout << co2 << std::endl;
	std::unique_ptr<CollisionPair> cp(new CollisionPair(co1,co2,point_of_intersection,normal_of_intersection,depth_of_intersection));
	//std::cout << " n: " << cp->get_normal_of_intersection().x << normal_of_intersection.y << normal_of_intersection.z <<  " (from simple_collision_hierarchy.cpp)"<< std::endl;
	input.push_back(std::move(cp));
	//std::cout << input[0]->get_first_collision_object() << std::endl;
      }
    }
  }
}

void SimpleCollisionHierarchy::add_object(CollisionObject *co) {
  CollisionHierarchyNode * coh = new CollisionHierarchyNode(co);
  coh->generate_bounding_geometry();
  assert(prog != NULL);
  //coh->create_bounding_geometry_object(prog);
  collision_hierarchy_nodes.push_back(coh);
  // Update the bounding geometries
}

void SimpleCollisionHierarchy::remove_object(CollisionObject * co) {
  for(auto it = collision_hierarchy_nodes.begin(); it != collision_hierarchy_nodes.end();) {
    if((*it)->get_collision_object() == co) {
      it = collision_hierarchy_nodes.erase(it);
      delete *it;
    } else {
      ++it;
    }
  }
  //TODO next update the bounding geometries (global ones)
}

void SimpleCollisionHierarchy::notify_about_mesh_change(CollisionObject * co) {
  //TODO
}

// Is this even necessary?
void SimpleCollisionHierarchy::notify_about_position_change(CollisionObject * co) {
  //TODO
}

SimpleCollisionHierarchy::~SimpleCollisionHierarchy() {
  for(auto child : collision_hierarchy_nodes) {
    delete child;
  }
}

void SimpleCollisionHierarchy::update_bounding_geometries() {
  for(auto child : collision_hierarchy_nodes) {
    child->update_bounding_geometry();
  }
}

SimpleCollisionHierarchy::SimpleCollisionHierarchy() {
}
