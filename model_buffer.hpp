#ifndef MODEL_BUFFER_HPP
#define MODEL_BUFFER_HPP
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "buffer.hpp"
#include "matrix_stack.hpp"
#include "global_context.hpp"

class ModelBuffer : public Buffer {
private:
  static const GLuint buffer_length = 4*(16+12);
  glm::mat4 model_matrix;
  
  glm::vec4 position;
  glm::fquat orientation;
  glm::vec3 scale;
  glm::vec3 center_of_mass;
  
protected:
  MatrixStack * model_stack;
  //MatrixStack * model_stack;
public:
  ModelBuffer();
  virtual void update_buffer_phase1(Object * parent);
  virtual void update_buffer_phase2(Object * parent);
  virtual void pre_update_function();
  virtual void post_update_function();

  virtual void pre_draw_function();
  virtual void post_draw_function();


  virtual ~ModelBuffer() {}
 
  glm::vec4 get_position_global() const {return model_matrix*glm::vec4(0,0,0,1);}
  glm::vec4 get_position() const {return position;}
  glm::fquat get_orientation() const {return orientation;}
  glm::vec3 get_scale() const {return scale;}
  glm::mat4 get_model_matrix() const {return model_matrix;}
  GLfloat get_uniform_scale();
  

  void set_position(glm::vec4 position) {this->position = position;}
  void set_orientation(glm::fquat orientation) {this->orientation = orientation;}
  void set_scale(glm::vec3 scale) {this->scale =scale;}
  void set_scale(GLdouble scale) {set_scale(glm::vec3(scale,scale,scale));}
  void update_model_matrix();
  void set_center_of_mass(const glm::vec3 & com);
  glm::vec3 get_center_of_mass() const;
  //virtual GLuint get_buffer_length();
};

#endif
