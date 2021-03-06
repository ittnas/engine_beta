#include "camera.hpp"
#include "global_context.hpp"
#include "world.hpp"

Camera::Camera() : Object(), Movable(), max_invocations(1),nbr_invocations_this_round(0) , rendering_function(0) {
  model_stack = GlobalContext::model_stack;
  try {
    this->view_buffer = new ViewBuffer();
    add_buffer(this->view_buffer);
  } catch (std::bad_alloc&) {
    std::cerr << "Failed to allocate memory." << std::endl;
    assert(0);
  }
}

/*
The main point of this function is to update the view_matrix and call view_buffer's update_buffer()-method to load the date into the GPU. However, it is possible to make different kinds of cameras using this method by genereating different view_matrices.
 */

void Camera::update_view_buffer() {
  // for example you can update the forward_dir-vector here.
  view_buffer->update_buffer(get_position());
}

/*
  With this function you can select where the output of the camera is rendered (framebuffer, texture, etc)
 */

void Camera::select_render_target() {
  // Example
  glBindFramebuffer(GL_FRAMEBUFFER,0); //Render to screen.
}

void Camera::draw_view(GLint tick, GLuint render_flag, GLuint current_program, Comparator comp,DrawingMode drawing_mode) {
  //get_world()->draw_screens(tick,render_flag,current_program,comp);
 
#ifdef DEBUG
  std::cout << "draw_screens disabled (From camera.cpp/draw_view)" << std::endl;
  std::cout << "camera.cpp: asking rendering function to perform the rendering." << rendering_function << std::endl;
#endif
  if(rendering_function != 0) {
    rendering_function->perform_rendering(tick,render_flag, current_program,comp,drawing_mode);
  } else {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    select_render_target();
    draw(tick,render_flag, current_program,comp,drawing_mode);
  }
}

void Camera::draw(GLint tick, GLuint render_flag, GLuint current_program, Comparator comp,DrawingMode drawing_mode) {
  //std::cout << "Camera drawing mode: " << drawing_mode << std::endl;
  get_world()->render(tick,render_flag,current_program,comp,drawing_mode);

}

void Camera::update_world() {
  update_view_pre();
  get_world()->update_all_buffers();
  update_view_post();
}

GLboolean Camera::not_too_many_invocations() const {
  return nbr_invocations_this_round < max_invocations;
}

void Camera::increment_invocations() {
  nbr_invocations_this_round++;
}

void Camera::update_view_pre() {
  //model_stack->push();
  //update_view_buffer();
  update_view_matrix();
  model_stack->load_matrix(get_view_matrix());
}

void Camera::update_view_post() {
  //model_stack->pop();
}

glm::mat4 Camera::get_view_matrix() const {
  return this->view_buffer->get_view_matrix();
}

void Camera::update_view_matrix() {
  this->view_buffer->update_view_matrix(get_position());
}

void Camera::set_rendering_function(RenderingFunction * rf) {
  std::cout << "camera.cpp: adding rendering function " << rf << std::endl;
  this->rendering_function = rf;
  if(rendering_function != 0) {
    rendering_function->set_target_camera(this);
  }
}

/*
RenderingFunction * Camera::get_rendering_function() const {
  return this->rendering_function;
}
*/
