#ifndef PARAMETRIC_PLOT_HPP
#define PARAMETRIC_PLOT_HPP

#include <memory>

#include "shape.hpp"
#include "parametric_surface_generator.hpp"

typedef glm::vec3 (*ParametricSurface)(GLfloat,GLfloat);

class ParametricPlot : public Shape {
private:
  //ParametricSurface function;
  std::unique_ptr<ParametricSurfaceGenerator> generator;
  glm::vec2 ulim;
  glm::vec2 rlim;
  GLuint nu;
  GLuint nr;
  GLboolean uclosed;
  GLboolean rclosed;
protected:
  virtual Mesh * create_mesh();
public:
  virtual ~ParametricPlot() {}
  ParametricPlot(ParametricSurface function,glm::vec2 ulim,glm::vec2 rlim,GLuint nu = 5,GLuint nr = 5, GLboolean uclosed = GL_FALSE,GLboolean rclosed = GL_FALSE);
  // ParametricPlot takes the ownership of the pointer
  ParametricPlot(std::unique_ptr<ParametricSurfaceGenerator> generator,glm::vec2 ulim,glm::vec2 rlim,GLuint nu = 5,GLuint nr = 5, GLboolean uclosed = GL_FALSE,GLboolean rclosed = GL_FALSE);
 
};


class ParametricSphere : public ParametricPlot {
private:
  glm::vec3 center;
  GLfloat radius;
public:
  ParametricSphere(glm::vec3 center,GLfloat radius,GLuint n);
  virtual ~ParametricSphere() {}
};

#endif
