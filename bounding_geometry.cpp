#define GLM_FORCE_RADIANS
#include <glm/gtx/norm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "bounding_geometry.hpp"
#include "parametric_plot.hpp"
#include "cuboid.hpp"
#include "parametric_functions.hpp"
#include "collision_detection_algorithms.hpp"

glm::mat4 BoundingGeometry::get_transformation_matrix(const glm::vec4 & position,const glm::fquat & orientation, const glm::vec3 & scale,const glm::vec3 & center_of_rotation) {
  glm::mat4 transformation = glm::mat4(1.0);
  glm::mat4 trans_mat(1.0);
  trans_mat[3] = glm::vec4(position.x+center_of_rotation.x,position.y+center_of_rotation.y,position.z+center_of_rotation.z,position.w);
  transformation = transformation*trans_mat;
  transformation = transformation * glm::mat4_cast(orientation);
  glm::mat4 scale_mat(1.0);
  scale_mat[0][0] = scale.x;
  scale_mat[1][1] = scale.y;
  scale_mat[2][2] = scale.z;
  transformation = transformation*scale_mat;
  glm::mat4 trans_back_mat(1.0);
  trans_back_mat[3] = glm::vec4(-center_of_rotation,1.0);
  transformation = transformation *trans_back_mat;
  return transformation;
}
/*
BoundingGeometry::BoundingGeometry(CollisionObject * collision_object, GLuint position_array_index) : target_object(collision_object), position_array_index(position_array_index) {
  recalculate_mesh(); // Calculates the mesh.
}
*/

/*
void BoundingGeometry::recalculate_mesh() {
  mesh.clear();
  target_object->get_transformed_mesh_points(mesh,position_array_index,COLLIDER|COLLIDABLE,OR);
}
*/

/*
BoundingSphere::BoundingSphere(CollisionObject * target_object, GLuint position_array_index) : BoundingGeometry(target_object,position_array_index) {}
*/

BoundingSphere::BoundingSphere(glm::vec4 center,GLfloat radius) : BoundingGeometry(),center(center),radius(radius),transformed_center(center),transformed_radius(radius) {
  
}

/** 
 * Transforms the bounding sphere using the given transformation parameters. Note, this only works for uniform scaling!
 * 
 */

void BoundingSphere::transform_geometry(const glm::vec4 & position,const glm::fquat & orientation,GLfloat scale,const glm::vec3 & cor) {
  glm::mat4 transformation = get_transformation_matrix(position,orientation,glm::vec3(scale,scale,scale),cor);
  transformed_center = transformation*center;
  //std::cout << "Center: x=" << transformed_center.x << ", y=" << transformed_center.y << ", z=" << transformed_center.z << " (from bounding_geometry.cpp/transfrom_geometry)" << std::endl;
  //glm::vec3 xcol(transformation[0]);
  //GLfloat scale_x = glm::l2Norm(xcol);
  transformed_radius = scale*radius;
}

GLboolean BoundingSphere::intersect(const BoundingSphere * bs,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) const {
  return CollisionDetectionAlgorithms::intersection_between(this,bs,calculate_collision_information,point_of_intersection,normal_of_intersection,depth_of_intersection);
  /*
  GLfloat distance2 = glm::distance2(this->get_center(),bs->get_center());
  GLfloat radius2 = std::pow(this->get_radius()+bs->get_radius(),2);
  if(distance2 > radius2) { // No intersection
    return std::make_tuple(GL_FALSE,glm::vec3(0,0,0),glm::vec3(0,0,0),0);
  } else { // intersection
    if(calculate_collision_information) {
      GLfloat distance = sqrt(distance2);
      GLfloat depth_of_intersection = sqrt(radius2) - distance;
      glm::vec4 center1 = this->get_center();
      glm::vec4 center2 = bs->get_center();
      glm::vec3 normal_of_intersection = glm::normalize(glm::vec3(this->get_center()) - glm::vec3(bs->get_center()));
      glm::vec3 point_of_intersection = glm::vec3(this->get_center() + (bs->get_center() - this->get_center())*this->get_radius()/distance);
      return std::make_tuple(GL_TRUE,point_of_intersection,normal_of_intersection,depth_of_intersection);
    } else {
      return std::make_tuple(GL_TRUE,glm::vec3(0,0,0),glm::vec3(0,0,0),0);
    }
  }
  */
}
GLboolean BoundingSphere::intersect(const BoundingAABB *aabb,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) const {
  return CollisionDetectionAlgorithms::intersection_between(this,aabb,calculate_collision_information,point_of_intersection,normal_of_intersection,depth_of_intersection);
}

GLboolean BoundingSphere::intersect(const BoundingOBB *obb,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) const {
  return CollisionDetectionAlgorithms::intersection_between(this,obb,calculate_collision_information,point_of_intersection,normal_of_intersection,depth_of_intersection);
}

Shape * BoundingSphere::create_shape() const {
  //return new ParametricPlot(ParametricFunctions::parametric_sphere,glm::vec2(0,2*M_PI),glm::vec2(0,M_PI),21,21,GL_TRUE,GL_FALSE);
  return new ParametricSphere(glm::vec3(center),radius,51);
}

GLboolean BoundingSphere::check_intersection(const BoundingGeometry & visitor,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) {
  return visitor.intersect(this,calculate_collision_information,point_of_intersection,normal_of_intersection,depth_of_intersection);
}

/*
  Initializes the axis-aligned bounding box.
 */
BoundingAABB::BoundingAABB(glm::vec3 min, glm::vec3 max) : BoundingGeometry(),min(min), max(max), trans_min(min), trans_max(max) {
  
}

/** 
 * Transforms the AABB using the given transformation parameters. Note, this only works for uniform scaling!
 * 
 */

void BoundingAABB::transform_geometry(const glm::vec4 & position,const glm::fquat & orientation,GLfloat scale,const glm::vec3 & cor) {
  // Follows Real-time collision detection p. 86
  glm::mat3 rotation_matrix = glm::mat3_cast(orientation);
  //First add translation
  for(GLint ii = 0; ii < 3; ii++) {
    trans_min[ii] = position[ii];
    trans_max[ii] = position[ii];
    for(GLint jj = 0; jj < 3; jj++) {
      GLfloat e = rotation_matrix[ii][jj] * min[jj];
      GLfloat f = rotation_matrix[ii][jj] * max[jj];
      if(e < f) {
	trans_min[ii] += e;
	trans_max[ii] += f;
      } else {
	trans_min[ii] += f;
	trans_max[ii] += e;
      }
    }
  }
  glm::vec3 diff = (trans_max - trans_min)/2.0f*(scale-1);
  trans_max += diff;
  trans_min -= diff;
}

GLboolean BoundingAABB::intersect(const BoundingSphere * bs,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) const {
  //glm::vec3 swapped_normal_of_intersection = (*normal_of_intersection)*-1.0f;
  //return CollisionDetectionAlgorithms::intersection_between(bs,this,calculate_collision_information,point_of_intersection,&swapped_normal_of_intersection,depth_of_intersection);
  GLboolean success =  CollisionDetectionAlgorithms::intersection_between(bs,this,calculate_collision_information,point_of_intersection,normal_of_intersection,depth_of_intersection);
  *normal_of_intersection = -1.0f*(*normal_of_intersection);
  return success;
  
}
GLboolean BoundingAABB::intersect(const BoundingAABB * aabb,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) const {
  return CollisionDetectionAlgorithms::intersection_between(this,aabb,calculate_collision_information,point_of_intersection,normal_of_intersection,depth_of_intersection);
}

GLboolean BoundingAABB::intersect(const BoundingOBB * obb,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) const {
  //TODO very inefficient
  BoundingOBB * temporaryOBB = new BoundingOBB(*this);
  GLboolean result = CollisionDetectionAlgorithms::intersection_between(temporaryOBB,obb,calculate_collision_information,point_of_intersection,normal_of_intersection,depth_of_intersection);
  delete temporaryOBB;
  return result;
}

GLboolean BoundingAABB::check_intersection(const BoundingGeometry & visitor,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) {
  return visitor.intersect(this,calculate_collision_information,point_of_intersection,normal_of_intersection,depth_of_intersection);
}

Shape * BoundingAABB::create_shape() const {
  return new Cuboid(max[0]-min[0],max[1]-min[1],max[2]-min[2]);
}

BoundingOBB::BoundingOBB(const BoundingAABB & source) : BoundingGeometry() {
  glm::vec3 half_widths = (source.get_trans_max() - source.get_trans_min())/static_cast<GLfloat>(2.0);
  glm::vec4 center = glm::vec4(source.get_trans_min() + half_widths,1);
  glm::mat3 axes = glm::mat3();
  this->half_widths = half_widths;
  this->transformed_half_widths = half_widths;
  this->center = center;
  this->transformed_center = center;
  this->axes = axes;
  this->transformed_axes = axes;
}

BoundingOBB::BoundingOBB(glm::vec4 center, glm::vec3 half_widths, glm::mat3 axes) : BoundingGeometry(), center(center), half_widths(half_widths),axes(axes), transformed_center(center),transformed_half_widths(half_widths), transformed_axes(axes) {
  /*
  for(GLuint ii = 0; ii < 3; ii++) {
    this->axes[ii] = axes[ii];
    this->transformed_axes[ii] = axes[ii];
  }
  */
}

GLboolean BoundingOBB::intersect(const BoundingSphere * bs,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) const {
  GLboolean success =  CollisionDetectionAlgorithms::intersection_between(bs,this,calculate_collision_information,point_of_intersection,normal_of_intersection,depth_of_intersection);
  *normal_of_intersection = -1.0f*(*normal_of_intersection);
  return success;
}

GLboolean BoundingOBB::intersect(const BoundingAABB * aabb,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) const {
  BoundingOBB * temporaryOBB = new BoundingOBB(*aabb);
  GLboolean result = CollisionDetectionAlgorithms::intersection_between(this,temporaryOBB,calculate_collision_information,point_of_intersection,normal_of_intersection,depth_of_intersection);
  delete temporaryOBB;
  return result;
}

GLboolean BoundingOBB::intersect(const BoundingOBB * obb,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) const {
  GLboolean success =  CollisionDetectionAlgorithms::intersection_between(this,obb,calculate_collision_information,point_of_intersection,normal_of_intersection,depth_of_intersection);
  return success;
}
GLboolean BoundingOBB::check_intersection(const BoundingGeometry & visitor,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) {
  return visitor.intersect(this,calculate_collision_information,point_of_intersection,normal_of_intersection,depth_of_intersection);
}
void BoundingOBB::transform_geometry(const glm::vec4 & position,const glm::fquat & orientation,GLfloat scale,const glm::vec3 & cor) {
  glm::mat4 transformation = get_transformation_matrix(position,orientation,glm::vec3(scale,scale,scale),cor);
  transformed_center = transformation*center;
  //This is very bad. mat3_cast is expensive, and it is done twice. OPTIMIZE. Maybe also pass model_matrix (if it is possible, I haven't thought it through yet).
  glm::mat3 rotation_matrix = glm::mat3_cast(orientation);
  /*
  for(GLuint ii=0;ii<3;ii++) {
    transformed_axes[ii] = rotation_matrix*axes[ii];
  }
  */
  transformed_axes = rotation_matrix*axes;
  transformed_half_widths = half_widths*scale;
}

Shape * BoundingOBB::create_shape() const {
  Cuboid * cuboid = new Cuboid(2*half_widths[0],2*half_widths[1],2*half_widths[2]);

  // Find orientation
  //glm::mat3 inverse_coordinate_transform = glm::inverse(axes);
  glm::mat3 inverse_coordinate_transform = axes; // WHY DOES ONE NOT NEED INVERSE???? (thas quite good though, if it's not an error)
  cuboid->get_mesh()->rotate_mesh(0,inverse_coordinate_transform);
  cuboid->get_mesh()->displace_mesh(0,4,glm::value_ptr(glm::vec4(glm::vec3(this->center),0)));
    //glm::fquat orientation = glm::quat_cast(inverse_coordinate_transform);
  return cuboid;
}

void BoundingOBB::get_vertices(std::vector<glm::vec3> & vertices) const {
  glm::vec3 center = glm::vec3(get_center());
  glm::mat3 axes =get_axes();
  glm::vec3 hw = get_half_widths();
  
  for(GLint ii = -1; ii < 2;ii=ii+2) {
    for(GLint jj = -1; jj < 2;jj=jj+2) {
      for(GLint kk = -1; kk < 2;kk=kk+2) {
	glm::vec3 vertex = center + static_cast<GLfloat>(-1*ii)*axes[0]*hw[0] + static_cast<GLfloat>(-1*jj)*axes[1]*hw[1] + static_cast<GLfloat>(-1*kk)*axes[2]*hw[2];
	vertices.push_back(vertex);
      }
    }
  }
}
