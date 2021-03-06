#include "collision_detector.hpp"

/*
void CollisionDetector::detect_collisions() {
  
}
*/

void CollisionDetector::manage() {
  if(ch != NULL) {
    ch->update_bounding_geometries();
    std::vector<std::unique_ptr<CollisionPair>> intersecting_pairs;
    ch->get_intersecting_pairs(intersecting_pairs);
    //std::vector<std::unque_ptr<CollisionPair>> ordered_pairs = solve_collision_priority(colliding_pairs); // Not needed for the simple example
    /*for(auto pair : ordered_pairs) {
      handle_collision(pair);
      }*/
    //std::unique_ptr<CollisionPair> pair = std::move(intersecting_pairs[0]);
    if(intersecting_pairs.size() > 0) {
      //std::cout << intersecting_pairs[0]->get_first_collision_object() << std::endl;
    }
    
    for(auto & pair : intersecting_pairs) {
      handle_collision(*pair);
    }
    for(auto & pair : intersecting_pairs) {
      pair->get_first_collision_object()->update_delayed_parameters();
      pair->get_second_collision_object()->update_delayed_parameters();
    }
    
    //for(auto & pair : intersecting_pairs) {
    /*
      std::vector<std::unique_ptr<CollisionPair>> new_intersecting_pairs;
      do {
	new_intersecting_pairs.clear();
	ch->get_intersecting_pairs(new_intersecting_pairs);
	for(auto & new_pair : new_intersecting_pairs) {
	  handle_collision(*new_pair);
	}
      } while(new_intersecting_pairs.size() > 0);
      //}
      */
    //std::vector<std::unique_ptr<CollisionPair>> new_pairs = ch->update_collision_pairs(ordered_pairs);
  } else {
    // DO nothing
  }
}

void CollisionDetector::handle_collision(const CollisionPair & cp) const {
  
  CollisionObject * co1 = cp.get_first_collision_object();
  CollisionObject * co2 = cp.get_second_collision_object();
  if(co1 != 0 || co2 != 0) {
    //assert(0);
  }
  co1->apply_collision_callback(cp);
  co2->apply_collision_callback(cp);
  //if(co1->accept_collision() && co2->accept_collision())
  co1->collision_with(*co2,cp);
  //std::cout << "Collision happened! (from collision_detector.cpp/handle_collision)" << std::endl;
  //TODO really handle the collision...
}

CollisionDetector::CollisionDetector() {
  ch = new SimpleCollisionHierarchy();
}

void CollisionDetector::register_object(Object * object) {
  CollisionObject * collision_object = dynamic_cast<CollisionObject *>(object);
  if(collision_object != NULL) {
    ch->add_object(collision_object);
  }
}

void CollisionDetector::deregister_object(Object * object) {
  CollisionObject * collision_object = dynamic_cast<CollisionObject *>(object);
  if(collision_object != NULL) {
    ch->remove_object(collision_object);
  }
}

