#ifndef FLYING_CAMERA_HPP
#define FLYING_CAMERA_HPP
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "camera.hpp"

class FlyingCamera : public Camera {

private:
  static GLfloat deg_to_rad_d2(GLfloat angle_in_degs);
public:
  virtual ~FlyingCamera() {}
  FlyingCamera();

  const glm::fquat create_quaternion(const glm::vec3 & axis, GLfloat angle);
  void yaw(GLfloat angle);
  void pitch(GLfloat angle);
  void roll(GLfloat angle);
  void move_to_point(glm::vec4 target_point);
  void move_to_direction(const glm::vec3 & direction, GLfloat distance);
  void move_forward(GLfloat distance);
  void move_backward(GLfloat distance);
};

#endif
