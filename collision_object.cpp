#include "collision_object.hpp"
#include "uniform_color_collision_object.hpp"
#include "collision_callback_function.hpp"
#include "collision_response_algorithms.hpp"

void CollisionObject::apply_collision_callback(const CollisionPair & cop) {
  //std::cout << "This manifests the problem! (from collision_object.cpp/apply_collision_callback" << std::endl;
  //std::cout << "Before " << collision_group_flag << std::endl;
  //std::cout << "Functions size " << collision_callback_functions.size() << std::endl;
  
  for(GLuint i = 0; i < collision_callback_functions.size(); i++) {
    collision_callback_functions[i]->apply(cop);
  }
  /*
  for(auto & cof : collision_callback_functions) {
    cof->apply(cop);
  }
  */
  
  //UniformColorCollisionObject * uuu  = dynamic_cast<UniformColorCollisionObject*>(cop.get_first_collision_object());
  //uuu->set_ambient_color(glm::vec4(1,0,0,1));
  
}

CollisionObject::CollisionObject() : Movable() {
  /*
  for(GLuint i = 0; i < 100; i++) {
      test_ints.push_back(1);
      std::cout << test_ints.size() << std::endl;
  }
  */
}


void CollisionObject::add_collision_callback_function(std::shared_ptr<CollisionCallbackFunction> ccbf) {
  collision_callback_functions.push_back(ccbf);
}

void CollisionObject::collision_with(CollisionObject & co,const CollisionPair & cp) {
  co.perform_collision_with(this,cp);
}

void CollisionObject::perform_collision_with(RigidBody * rb, const CollisionPair & cp) {
  CollisionResponseAlgorithms::collision_between(this,rb,cp);
}

void CollisionObject::perform_collision_with(CollisionObject * co,const CollisionPair & cp) {
  CollisionResponseAlgorithms::collision_between(this,co,cp);
}
