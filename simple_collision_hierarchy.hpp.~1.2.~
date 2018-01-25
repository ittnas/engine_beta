#ifndef SIMPLE_COLLISION_HIERARCHY_HPP
#define SIMPLE_COLLISION_HIERARCHY_HPP

#include <vector>

#include "collision_hierarchy.hpp"
#include "collision_hierarchy_node.hpp"

class SimpleCollisionHierarchy : public CollisionHierarchy {
private:
  std::vector<CollisionHierarchyNode * > collision_hierarchy_nodes; // Maybe a std::unordered_map<CollisionObject *,CollisionHierarchyNode *> would be better.
public:
  SimpleCollisionHierarchy();
  void get_intersecting_pairs(std::vector<std::unique_ptr<CollisionPair>> & input);
  void add_object(CollisionObject *);
  void remove_object(CollisionObject *);
  void notify_about_mesh_change(CollisionObject *);
  void notify_about_position_change(CollisionObject *);
  virtual ~SimpleCollisionHierarchy();
  void update_bounding_geometries();
};
#endif
