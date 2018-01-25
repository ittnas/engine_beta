#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "object.hpp"
#include "movable.hpp"
#include "view_buffer.hpp"
#include "rendering_function.hpp"

class Camera : virtual public Object, virtual public Movable {
private:
  //GLboolean active;
  ViewBuffer * view_buffer;
  const GLuint max_invocations;
  GLuint nbr_invocations_this_round;
  MatrixStack * model_stack;
  glm::mat4 get_view_matrix() const;
  RenderingFunction * rendering_function;
protected:
  glm::vec3 get_up_dir() {return view_buffer->get_up_dir();}
  glm::vec3 get_forward_dir() {return view_buffer->get_forward_dir();}
  glm::fquat get_forward() {return view_buffer->get_forward();}
  glm::fquat get_up() {return view_buffer->get_up();}
  void set_up(glm::fquat up) {view_buffer->set_up(up);}
  void set_forward(glm::fquat forward) {view_buffer->set_forward(forward);}
public:
  Camera();
  virtual ~Camera() {}
  virtual void update_view_buffer(); // If you want to make a different camera, override this.
virtual void update_view_matrix();
  //void activate() {this->view_buffer->activate();}
  //void deactivate() {this->view_buffer->deactivate();}
  virtual void select_render_target();
  void draw_view(GLint tick, GLuint render_flag = -1, GLuint current_program = 0, Comparator comp = ANY,DrawingMode drawing_mode = NOTSET);
  GLboolean not_too_many_invocations() const;
  void increment_invocations(); 
  void update_view_pre();
  void update_view_post();
  void set_rendering_function(RenderingFunction * rf);
  //RenderingFunction * get_rendering_function() const;
  void draw(GLint tick, GLuint render_flag = -1, GLuint current_program = 0, Comparator comp = ANY,DrawingMode drawing_mode = NOTSET);
  void update_world();
};

#endif







