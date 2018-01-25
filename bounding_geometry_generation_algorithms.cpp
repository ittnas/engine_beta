#define GLM_FORCE_RADIANS
#include <glm/gtx/norm.hpp>

#include "bounding_geometry_generation_algorithms.hpp"

BoundingSphere * BGAlgorithm::ritters_algorithm(const std::vector<glm::vec4> & points) {
  assert(points.size() > 1);
  glm::vec4 ip = points[0];
  glm::vec4 tp;
  GLfloat distance = 0;
  // Find a good initial guess. This might not be the optimal way of doing it.
  for(GLuint i = 1; i < points.size(); i++) {

    GLfloat next_distance = glm::distance2(ip,points[i]);
    if(next_distance>distance) {
      tp=points[i];
      distance = next_distance;
    }
  }

  for(GLuint i = 0; i < points.size(); i++) {
    GLfloat next_distance = glm::distance2(tp,points[i]);
    if(next_distance>distance) { // distance not reinitialized, because we know already that the first point has at least that distance
      ip=points[i];
      distance = next_distance;
    }
  }
  
  glm::vec4 center = (ip+tp)/(GLfloat)2.0;
  GLfloat radius2 = glm::distance2(ip,tp)/4.0;
  //GLfloat radius = std::sqrt(radius2)/2.0;
  GLfloat radius = std::sqrt(radius2);
  for(GLuint i = 0; i < points.size(); i++) {
    GLfloat distance_from_center2 = glm::distance2(center,points[i]);
    if(distance_from_center2 > radius2) {
      // The point is not in the sphere
      GLfloat distance_from_center = std::sqrt(distance_from_center2);
      center = center + (points[i] - center)/GLfloat(2.0)*(distance_from_center-radius)/distance_from_center;
      radius = radius + (distance_from_center-radius)/2.0;
      radius2 = radius*radius;
      //center = center + (points[i] - center)/GLfloat(2.0);
    }
  }
  //std::cout << "radius: " << radius << std::endl;
  return new BoundingSphere(center,radius);
}

BoundingAABB * BGAlgorithm::boundingAABB_from_points(const std::vector<glm::vec4> & points) {
  glm::vec3 min = glm::vec3(std::numeric_limits<GLfloat>::max(),std::numeric_limits<GLfloat>::max(),std::numeric_limits<GLfloat>::max());
  glm::vec3 max = glm::vec3(-std::numeric_limits<GLfloat>::max(),-std::numeric_limits<GLfloat>::max(),-std::numeric_limits<GLfloat>::max());
  for(GLuint ii = 0; ii < points.size(); ii++) {
    for(GLuint jj = 0; jj < 3; jj++) {
      if(points[ii][jj] < min[jj]) {
	min[jj] = points[ii][jj];
      }
      if(points[ii][jj] > max[jj]) {
	max[jj] = points[ii][jj];
      }
    }
  }
  return new BoundingAABB(min,max);
}

BoundingOBB * BGAlgorithm::simple_bounding_OBB(const std::vector<glm::vec4> & points) {
  // Algorithms from rtcd p. 107
  // First calcualte AABB
  glm::vec3 center;
  glm::vec3 half_widths;
  glm::mat3 axes;

  glm::vec3 min = glm::vec3(std::numeric_limits<GLfloat>::max(),std::numeric_limits<GLfloat>::max(),std::numeric_limits<GLfloat>::max());
  glm::vec3 max = glm::vec3(-std::numeric_limits<GLfloat>::max(),-std::numeric_limits<GLfloat>::max(),-std::numeric_limits<GLfloat>::max());
  GLuint max_index[3];
  GLuint min_index[3];
  for(GLuint ii = 0; ii < points.size(); ii++) {
    for(GLuint jj = 0; jj < 3; jj++) {
      if(points[ii][jj] < min[jj]) {
	min[jj] = points[ii][jj];
	min_index[jj] = ii;
      }
      if(points[ii][jj] > max[jj]) {
	max[jj] = points[ii][jj];
	max_index[jj] = ii;
      }
    }
  }
  // Next find the largest axis
  GLuint max_axis;
  GLfloat max_distance = std::numeric_limits<GLfloat>::min();
  for(GLuint ii = 0; ii < 3;ii++) {
    if(max[ii] - min[ii] > max_distance) {
      max_distance = max[ii] - min[ii];
      max_axis = ii;
    }
  }
  glm::vec3 first_axis = glm::vec3(points[max_index[max_axis]] - points[min_index[max_axis]]);
  glm::vec3 first_axis_norm = glm::normalize(first_axis);
  axes[0] = first_axis_norm;
  half_widths[0] = glm::length(first_axis)/2.0f;
  glm::vec3 ref1 = glm::vec3(points[min_index[max_axis]]);
  
  // Projection: p1 - p1.n1*n1
  std::vector<glm::vec3> projected_points;
  projected_points.reserve(points.size());
  for(GLuint ii = 0; ii < points.size(); ii++) {
    glm::vec3 from_ref1 = glm::vec3(points[ii]) - ref1;
    //projected_points.push_back(glm::vec3(points[ii]) - glm::dot(glm::vec3(points[ii]),first_axis_norm)*first_axis_norm);
    projected_points.push_back(from_ref1 - glm::dot(glm::vec3(from_ref1),first_axis_norm)*first_axis_norm);
    //std::cout << "x: " << projected_points[ii].x << " y:" << projected_points[ii].y << " z: " << projected_points[ii].z << std::endl;
  }
  // Next find AABB from the projected points
  
  min = glm::vec3(std::numeric_limits<GLfloat>::max(),std::numeric_limits<GLfloat>::max(),std::numeric_limits<GLfloat>::max());
  max = glm::vec3(-std::numeric_limits<GLfloat>::max(),-std::numeric_limits<GLfloat>::max(),-std::numeric_limits<GLfloat>::max());
  for(GLuint ii = 0; ii < projected_points.size(); ii++) {
    for(GLuint jj = 0; jj < 3; jj++) {
      if(projected_points[ii][jj] < min[jj]) {
	min[jj] = projected_points[ii][jj];
	min_index[jj] = ii;
      }
      if(projected_points[ii][jj] > max[jj]) {
	max[jj] = projected_points[ii][jj];
	max_index[jj] = ii;
      }
    }
  }
  // Again find the max axis
  max_distance = std::numeric_limits<GLfloat>::min();
  for(GLuint ii = 0; ii < 3;ii++) {
    if(max[ii] - min[ii] > max_distance) {
      max_distance = max[ii] - min[ii];
      max_axis = ii;
    }
  }
  glm::vec3 second_axis = glm::vec3(projected_points[max_index[max_axis]] - projected_points[min_index[max_axis]]);
  glm::vec3 second_axis_norm = glm::normalize(second_axis);
  axes[1] = second_axis_norm;
  half_widths[1] = glm::length(second_axis)/2.0f;
  //glm::vec3 ref2 = projected_points[min_index[max_axis]] + axes[0]*(glm::dot(ref1,axes[0]));
  glm::vec3 ref2 = projected_points[min_index[max_axis]];

  glm::vec3 third_axis_norm = glm::normalize(glm::cross(first_axis,second_axis));

  GLfloat max_distance_along_normal = std::numeric_limits<GLfloat>::min();
  GLfloat min_distance_along_normal = std::numeric_limits<GLfloat>::max();
  //GLuint max_distance_index;
  GLuint min_distance_index;
  for(GLuint ii=0; ii<points.size(); ii++) {
    GLfloat distance_along_normal = glm::dot(projected_points[ii],third_axis_norm);
    if(distance_along_normal > max_distance_along_normal) {
      max_distance_along_normal = distance_along_normal;
      //max_distance_index = ii;
    }
    if(distance_along_normal < min_distance_along_normal) {
      min_distance_along_normal = distance_along_normal;
      min_distance_index = ii;
    }
  }
  //glm::vec3 ref3 = projected_points[min_distance_index] + axes[0]*(glm::dot(ref1,axes[0]));
  glm::vec3 ref3 = projected_points[min_distance_index];
  axes[2] = third_axis_norm;
  half_widths[2] = (max_distance_along_normal-min_distance_along_normal)/2.0f;
  //glm::vec3 third_axis = (max_distance_along_normal - min_distance_along_normal)*third_axis_norm;
  //center = ref1 + axes[0]*half_widths[0] + (ref2 -ref1 + axes[1]*half_widths[1]) + (ref3 -ref1 + axes[2]*half_widths[2]);
  //center = ref1 + axes[0]*half_widths[0] + (ref2 + axes[1]*half_widths[1]) + (ref3 + axes[2]*half_widths[2]);
  center = ref1 + axes[0]*half_widths[0] + (axes[1]*half_widths[1] + glm::dot(ref2,axes[1])*axes[1]) + (axes[2]*half_widths[2] + glm::dot(ref3,axes[2])*axes[2]);
  BoundingOBB * bobb = new BoundingOBB(glm::vec4(center,1),half_widths,axes);
  for(GLuint ii=0; ii < 3; ii++) {
    std::cout << "Axis " << ii << ": " << axes[ii].x << ", " << axes[ii].y << ", " << axes[ii].z << " Halfwidth: " << half_widths[ii] << std::endl;
  }
  std::cout << "Center: " << center.x << ", " << center.y << ", " << center.z << std::endl;
  std::cout << "From bounding_geometry_generation_algorithms.cpp/simple_bounding_OBB" << std::endl;
  return bobb;
}



