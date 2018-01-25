#ifndef FIREWORK_SHAPE_HPP
#define FIREWORK_SHAPE_HPP

#include "shape.hpp"

class FireworkShape : public Shape {
private:
  GLuint n;
  GLuint seed;
  GLfloat mean_velocity;
  GLfloat stdev_velocity;
  glm::vec3 color;
  glm::vec3 color_stdev;

  GLfloat get_random_nbr(GLfloat min, GLfloat max);
  void get_gaussian_random_nbr(GLfloat mean, GLfloat stdev, GLuint n, GLfloat * output, GLuint offset = 1);
  void get_gaussian_random_nbr_3d(GLfloat mean, GLfloat stdev, GLuint n, GLfloat * output);
  void get_uniform_random_nbr(GLfloat lower, GLfloat upper, GLuint n, GLfloat * output);
protected:
  virtual Mesh * create_mesh();
public:
  virtual ~FireworkShape() {}
  FireworkShape(GLuint n, GLfloat mean_velocity,GLfloat stdev_velocity,glm::vec3 color,glm::vec3 color_stdev = glm::vec3(0,0,0),GLuint seed = -1);
};

#endif
