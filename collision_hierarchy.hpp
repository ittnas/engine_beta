#ifndef COLLISION_HIERARCHY_HPP
#define COLLISION_HIERARCHY_HPP

#include <GL/glew.h>
#include <memory>
#include <vector>
#include "collision_pair.hpp"
#include "program.hpp"

class CollisionHierarchy {
protected:
  GLboolean render_bounding_geometry;
  Program * prog = 0;
public:
  CollisionHierarchy() {}
  virtual void get_intersecting_pairs(std::vector<std::unique_ptr<CollisionPair>> & input) = 0;
  virtual void add_object(CollisionObject *) = 0;
  virtual void remove_object(CollisionObject *) = 0;
  virtual void notify_about_mesh_change(CollisionObject *) = 0;
  virtual void notify_about_position_change(CollisionObject *) = 0; // Is this even necessary?
  virtual ~CollisionHierarchy() {}
  void enable_bounding_geometry_rendering() {this->render_bounding_geometry = GL_TRUE;}
  void disable_bounding_geometry_rendering() {this->render_bounding_geometry = GL_FALSE;}
  virtual void update_bounding_geometries() = 0;
  //virtual std::vector<std::unique_ptr<CollisionPair>> update_collision_pairs(const std::vector<std::unique_ptr<CollisionPair>> & resolved_pairs) = 0;
  void set_bounding_geometry_object_program(Program * prog) {this->prog = prog;}
};
#endif
