#ifndef SIMPLE_COLLISION_DETECTOR_HPP
#define SIMPLE_COLLISION_DETECTOR_HPP

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

#include "aabb.hpp"
#include "mesh.hpp"
#include "object.hpp"
#include "uniform_material.hpp"
#include "rigid_body.hpp"
#include "world.hpp"

class SimpleCollisionDetector {
private:
  std::vector<UniformMaterial *> objects;
  std::vector<Aabb*> Aabbs_from_mesh;
  std::vector<Aabb*> Aabbs;
public:
  static Aabb * generate_Aabb(Mesh * mesh, GLuint position_array_index);
  static void multiply_Aabb_with_matrix(Aabb * source, Aabb * destionation, const glm::mat4 & model_matrix);
  void add_object(Object * object);
  void detect_collisions();
  void apply_transformations();
  GLboolean collision_takes_place(Aabb * obje1, Aabb* obj2);
  SimpleCollisionDetector() {}
};

#endif
