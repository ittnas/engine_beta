#ifndef PARAMETRIC_SPHERE_HPP
#define PARAMETRIC_SPHERE_HPP
#include "shape.hpp"
#include "parametric_plot.hpp"

/*
  THIS IS A BOMB. DELETING THIS OBJECT WILL MAKE THE WORLD BURN.
  Sphere is constructed by making another parametric plot, and then giving its mesh to this shape. Thus the mesh will be deleted twice. This is not done in now, resulting in a memory leak.
 */
class ParametricSphere : public Shape {
private:
  glm::vec3 plotting_function(GLfloat u,GLfloat r);
  ParametricPlot * pl;
  glm::vec3 center;
  GLfloat radius;
  GLuint n;
protected:
  virtual Mesh * create_mesh();
public:
  virtual ~ParametricSphere() {}
  ParametricSphere(glm::vec3 center,GLfloat radius, GLuint n);
};
#endif

