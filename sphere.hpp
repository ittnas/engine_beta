#ifndef SPHERE_HPP
#define SPHERE_HPP

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>

#include "shape.hpp"

class Sphere : public Shape {
public:
Sphere(glm::vec3 center, GLfloat radius,GLuint n);
virtual ~Sphere() {}
protected:
virtual Mesh * create_mesh();
};

#endif
