#ifndef PROJECTION_BUFFER_HPP
#define PROJECTION_BUFFER_HPP

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "buffer.hpp"
#include "matrix_stack.hpp"


class ProjectionBuffer : public Buffer {
private:
  static const GLuint buffer_length = 4*16+1*4; // Just the projection matrix + time
  glm::mat4 projection_matrix;
  GLfloat tick;
protected:
  MatrixStack * projection_stack; // This is not enabled at the moment. It just hangs around.
public:
  ProjectionBuffer();
  virtual void update_buffer_phase1(Object * parent);
  //virtual void update_buffer_phase2(Object * parent);
  //virtual void pre_update_function();
  //virtual void post_update_function();
  virtual ~ProjectionBuffer();
  void set_projection(glm::mat4 projection_matrix);
  void set_time(GLfloat tick) {this->tick = tick;}
  static glm::mat4 create_perspective_projection_matrix(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearval, GLfloat farval,GLfloat inf);
};


#endif
