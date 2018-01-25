#ifndef COLLISION_OBJECT_HPP
#define COLLISION_OBJECT_HPP

#include <memory>
#include <vector>
#include "movable.hpp"
#include "render_flag.hpp"
#include "bounding_geometry_type.hpp"
#include "collision_pair.hpp"
#include "collision_callback_function.hpp"
#include "physics_object.hpp"

class RigidBody;
//class CollisionCallbackFunction;

class CollisionObject : virtual public Movable {
private:
  GLuint accepted_target_collision_groups;
  GLuint collision_group_flag;
  BoundingGeometryType preferred_bounding_volume;
  std::vector<std::shared_ptr<CollisionCallbackFunction>> collision_callback_functions;

  
  //std::vector<CollisionCallBackFunction(CollisionPair) *>
protected:
  void add_collision_callback_function(std::shared_ptr<CollisionCallbackFunction> ccbf);
public:
  CollisionObject();
  virtual ~CollisionObject() {}
  void set_preferred_bounding_volume(BoundingGeometryType bvt) {this->preferred_bounding_volume = bvt;}
  BoundingGeometryType get_preferred_bounding_volume() const {return preferred_bounding_volume;}
  void set_collision_group_flag(GLuint collision_group_flag);
  GLboolean accept_collision(CollisionObject * co);
  void apply_collision_callback(const CollisionPair &);
  virtual void update_delayed_parameters() {} // GET RID OF DELAYED UPDATE. DID NOT WORK AS INTENDED!!

  // Double dispatch
  virtual void collision_with(CollisionObject & co,const CollisionPair & cp);
  virtual void perform_collision_with(RigidBody * rb, const CollisionPair & cp);
  virtual void perform_collision_with(CollisionObject * rb, const CollisionPair & cp);
};

#endif
