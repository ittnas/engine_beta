#ifndef PARAMETRIC_SURFACE_GENERATOR_HPP
#define PARAMETRIC_SURFACE_GENERATOR_HPP

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <GL/glew.h>

typedef glm::vec3 (*ParametricSurface)(GLfloat,GLfloat);

class ParametricSurfaceGenerator {
protected:
  ParametricSurfaceGenerator() {}
public:
  virtual glm::vec3 get_value_at(GLfloat u, GLfloat r) const = 0;
};

class ParametricSphereGenerator : public ParametricSurfaceGenerator {
private:
  glm::vec3 center;
  GLfloat radius;
public:
  ParametricSphereGenerator(glm::vec3 center, GLfloat radius);
  virtual glm::vec3 get_value_at(GLfloat u, GLfloat r) const;
};

class ParametricSurfaceGeneratorFromFunction : public ParametricSurfaceGenerator{
private:
  ParametricSurface function;
public:
  ParametricSurfaceGeneratorFromFunction(ParametricSurface);
  virtual glm::vec3 get_value_at(GLfloat u, GLfloat r) const;
};

#endif
