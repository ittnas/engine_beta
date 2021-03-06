#ifndef COLLISION_DETECTOR_HPP
#define COLLISION_DETECTOR_HPP

#include <vector>
#include <memory>
#include "manager.hpp"
#include "collision_object.hpp"
#include "collision_pair.hpp"
#include "collision_hierarchy.hpp"
#include "simple_collision_hierarchy.hpp"
//#include "collision_handler.hpp"

/*
  //TODO Change collision detector to be interface so that it does not create the hierarchy. The derived classes can then implement their own hiererchies, or decide not to even have one.
 */

class CollisionDetector : public Manager {
private:
  //CollisionHandler * collision_handler;
  CollisionHierarchy * ch;
protected:
  std::vector<CollisionObject *> objects;
  std::vector<CollisionPair *> generate_collision_pairs_broad_phase(GLboolean calculate_intersection_information);
  std::vector<CollisionPair *> generate_collision_pairs_near_phase(std::vector<CollisionPair *> & collision_pairs,GLboolean calculate_intersecion_information);
  virtual void handle_collision(const CollisionPair & cp) const;
  //virtual std::vector<std::unique_ptr<CollisionPair>> solve_collision_priority(std::vector<std::unique_ptr<CollisionPair>> & );
public:
  //void detect_collisions(); // handled by manage
  void add_object(CollisionObject * object); // should this actually be virtual, in order to check the real correct type of the object...
  void set_bounding_geometry_object_program(Program * prog) {ch->set_bounding_geometry_object_program(prog);}
  void remove_object(CollisionObject * object);
  CollisionDetector();
  virtual ~CollisionDetector() {delete ch;}
  //virtual void generate_bounding_volume();
  void register_object(Object * object);
  void deregister_object(Object * object);
  void manage();
};
#endif
