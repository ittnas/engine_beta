#ifndef COLLISION_HIERARCHY_NODE_HPP
#define COLLISION_HIERARCHY_NODE_HPP

#include "collision_object.hpp"
#include "bounding_geometry.hpp"
#include "movable.hpp"

class CollisionHierarchyNode {
private:
  BoundingGeometry * local_bounding_geometry = 0;
  CollisionObject * collision_object = 0;
  Movable * bounding_geometry_object = 0;
  BoundingGeometry * global_bounding_geometry = 0;
  //std::vector<Program **> bounding_geometry_object_programs;
public:
  CollisionHierarchyNode(CollisionObject *);
  ~CollisionHierarchyNode() {}
  void generate_bounding_geometry() {generate_bounding_geometry(std::vector<BoundingGeometry *>());}
  virtual void generate_bounding_geometry(const std::vector<BoundingGeometry *> & source_geometries);
  virtual GLboolean create_bounding_geometry_object(Program * prog);
  //void update_bounding_geometry_object();
  void update_bounding_geometry();
  //TODO RETURN GLOBAL BINDING GEOMTETRY!!!
  BoundingGeometry * get_bounding_geometry() {return local_bounding_geometry;}
  CollisionObject * get_collision_object() const {return collision_object;}
  //void set_bounding_geometry_object_program(Program ** prog, GLuint program_position = 0);
};

#endif
