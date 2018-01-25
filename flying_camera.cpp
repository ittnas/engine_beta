#include "flying_camera.hpp"

/** 
 * Creates the orientation quaternion from given axis and angle. The axis is NOT normalized by this function. If a unit quaternion is required, the axis should be normalized in advance.
 * 
 * @param axis Axis around which rotation rotation is performed.
 * @param angle Angle of rotation in degrees.
 * 
 * @return Orientation quaternion representing the rotation around given axis by given degrees.
 */
const glm::fquat FlyingCamera::create_quaternion(const glm::vec3 & axis, GLfloat angle) {
  GLfloat angled2 = deg_to_rad_d2(angle);
  glm::vec3 body = axis*sinf(angled2);
  GLfloat scalar = cosf(angled2);

  return glm::fquat(scalar,body.x,body.y,body.z);
}

/** 
 * Rotates the camera around the y-axis by given angle. Note that the axis is specifed in camera space.
 * 
 * @param angle Angle of rotation in degrees.
 */

void FlyingCamera::yaw(GLfloat angle) {
  glm::fquat quaternion = create_quaternion(get_up_dir(),angle);
  set_forward(quaternion*get_forward());
  set_up(quaternion*get_up());
}

/**
 * Rotates the camera around the z-axis by given angle. Note that the axis is specifed in camera space.
 *
 * @param angle Angle of rotation in degrees.
 */

void FlyingCamera::pitch(GLfloat angle) {
  glm::fquat quaternion = create_quaternion(glm::cross(get_forward_dir(),get_up_dir()),angle);
  set_forward(quaternion*get_forward());
  set_up(quaternion*get_up());
}

/**
 * Rotates the camera around the x-axis by given angle. Note that the axis is specifed in camera space.
 *
 * @param angle Angle of rotation in degrees.
 */

void FlyingCamera::roll(GLfloat angle) {
  glm::fquat quaternion = create_quaternion(get_forward_dir(),angle);
  set_up(quaternion*get_up());
  set_forward(quaternion * get_forward());
}

void FlyingCamera::move_to_point(glm::vec4 target_point) {
  set_position(target_point);
}

void FlyingCamera::move_to_direction(const glm::vec3 & direction, GLfloat distance) {
  glm::vec3 normalized_direction = glm::normalize(direction);
  glm::vec4 position = get_position();
  glm::vec3 position3(position.x,position.y,position.z);
    
  glm::vec3 new_position = position3 + normalized_direction*distance;
  set_position(glm::vec4(new_position.x,new_position.y,new_position.z,1));
}

void FlyingCamera::move_forward(GLfloat distance) {
  glm::mat3 rotation = glm::mat3_cast(get_forward());
  // The order of multiplication is reversed... now it seems to be working quite fine.
  glm::vec3 axis_of_motion = get_forward_dir()*rotation;
  // axis_of_motion should be normalized
  glm::vec3 position3 = glm::vec3(get_position().x,get_position().y,get_position().z);
  glm::vec3 new_position = position3 + axis_of_motion*distance;
  set_position(glm::vec4(new_position.x,new_position.y,new_position.z,1));

}
void FlyingCamera::move_backward(GLfloat distance) {
  move_forward(-distance);
}

GLfloat FlyingCamera::deg_to_rad_d2(GLfloat angle_in_degs) 
{
  return angle_in_degs*0.0087266461111111; // \theta_rad/2 = 2*pi/360*\theta_deg/2
}

FlyingCamera::FlyingCamera() : Camera() {

}
