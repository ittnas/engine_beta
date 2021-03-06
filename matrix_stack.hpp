#ifndef MATRIX_STACK_HPP
#define MATRIX_STACK_HPP

#include <GL/glew.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

#include <stack>

class MatrixStack {
public:

  MatrixStack();
  void pop();
  void push();
  void load_identity();
  void translate(const glm::vec4 &);
  void translate(const glm::vec3 &);
  void load_matrix(glm::mat4);
  void rotate(GLfloat,const glm::vec3 &);
  void rotate(const glm::fquat &);
  void scale(const glm::vec3 & scale);
  void scale(const GLdouble scale);
  glm::mat4 top();

  void mult_array(const std::vector<GLfloat> & input, std::vector<GLfloat> output, GLuint size = 4);

  void mult_array(const std::vector<glm::vec3> & input, std::vector<glm::vec3> & output);

  void mult_array(const std::vector<glm::vec4> & input, std::vector<glm::vec4> & output);
  //void set(glm::mat4);

  void mult_array(const GLfloat * input, GLfloat * output, GLuint size, GLuint nbr_elements);

  void mult_normal_array(const GLfloat * input, GLfloat * output, GLuint nbr_elements);

private:
  std::stack<glm::mat4> stack;

};

#endif
