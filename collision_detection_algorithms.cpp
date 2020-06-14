#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include "collision_detection_algorithms.hpp"

GLboolean CollisionDetectionAlgorithms::intersection_between(const BoundingSphere * bs1, const BoundingSphere * bs2,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection) {
  GLfloat distance2 = glm::distance2(bs1->get_center(),bs2->get_center());
  GLfloat radius2 = std::pow(bs1->get_radius()+bs2->get_radius(),2);
  if(distance2 > radius2) { // No intersection
    //return std::make_tuple(GL_FALSE,glm::vec3(0,0,0),glm::vec3(0,0,0),0);
    return GL_FALSE;
  } else { // intersection
    if(calculate_collision_information) {
      GLfloat distance = sqrt(distance2);
      *depth_of_intersection = sqrt(radius2) - distance;
      glm::vec4 center1 = bs1->get_center();
      glm::vec4 center2 = bs2->get_center();
      *normal_of_intersection = glm::normalize(glm::vec3(bs2->get_center()) - glm::vec3(bs1->get_center()));
      *point_of_intersection = glm::vec3(bs1->get_center() + (bs2->get_center() - bs1->get_center())*bs2->get_radius()/distance);
      return GL_TRUE;
    } else {
      return GL_TRUE;
    }
  }
}

GLboolean CollisionDetectionAlgorithms::intersection_between(const BoundingSphere * bs, const BoundingAABB * box,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) {
  glm::vec3 center(bs->get_center());
  GLfloat sq_dist = 0.0f;
  for (GLint ii = 0; ii < 3; ii++) {
// For each axis count any excess distance outside box extents
    GLfloat v = center[ii];
    if (v < box->get_trans_min()[ii]) sq_dist += (box->get_trans_min()[ii] - v) * (box->get_trans_min()[ii] - v);
    if (v > box->get_trans_max()[ii]) sq_dist += (v - box->get_trans_max()[ii]) * (v - box->get_trans_max()[ii]);
  }
  if(sq_dist <= bs->get_radius()*bs->get_radius()) { // Collision
    if(calculate_collision_information) {
      // Get the closest point
      for (GLint ii = 0; ii < 3; ii++) {
	GLfloat v = center[ii];
	if (v < box->get_trans_min()[ii]) {
	  v = box->get_trans_min()[ii];
	}
	if (v > box->get_trans_max()[ii]) {
	  v = box->get_trans_max()[ii];
	}
	(*point_of_intersection)[ii] = v;
      }
      *depth_of_intersection = bs->get_radius() - std::sqrt(sq_dist);
      *normal_of_intersection = glm::normalize(*point_of_intersection - center);
    }
    return GL_TRUE;
  } else {
    return GL_FALSE;
  }
}




GLboolean CollisionDetectionAlgorithms::intersection_between(const BoundingAABB * a, const BoundingAABB * b,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection, glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection) {
  // From RTCD p. 79
  // Exit with no intersection if separated along an axis
  for(GLint ii = 0; ii < 3; ii++) {
  if (a->get_trans_max()[ii] < b->get_trans_min()[ii] || a->get_trans_min()[ii] > b->get_trans_max()[ii]) return GL_FALSE;
  }
// Overlapping on all axes means AABBs are intersecting
  if(calculate_collision_information) {
    glm::vec3 diff;
    GLfloat min_val = std::numeric_limits<GLfloat>::max();
    GLint min_index;
    for(GLint ii = 0; ii < 3; ii++) {
      GLfloat max = std::min(a->get_trans_max()[ii],b->get_trans_max()[ii]);
      GLfloat min = std::max(a->get_trans_min()[ii],b->get_trans_min()[ii]);
      diff[ii] = max-min;
      if(diff[ii] < min_val) {
	min_val = diff[ii];
	min_index = ii;
      }
    }
    assert(min_index >= 0);
    *normal_of_intersection = glm::vec3(0,0,0);
    for(GLint ii = 0; ii < 3; ii++) {
      if(ii == min_index) {
	*depth_of_intersection = diff[ii];
	if(a->get_trans_max()[ii] <= b->get_trans_min()[ii]) {
	  (*point_of_intersection)[ii] = a->get_trans_min()[ii];
	  (*normal_of_intersection)[ii] = -1;
	} else {
	  //a->get_trans_max()[ii];
	  (*normal_of_intersection)[ii] = 1;
	}
      }
      (*point_of_intersection)[ii] = a->get_trans_max()[ii] > b->get_trans_max()[ii] ? b->get_trans_max()[ii] - diff[ii]/2.0 : a->get_trans_max()[ii] - diff[ii]/2.0;
    }
  }
  return GL_TRUE;
}

void CollisionDetectionAlgorithms::closest_point_on_obb_to_point(const glm::vec3 & p, const BoundingOBB & b,glm::vec3 & q) {
  // After RTCT p. 133
  glm::vec3 c = glm::vec3(b.get_center());
  glm::vec3 d = p - c;
  q = c;
  glm::mat3 axes = b.get_axes();
  glm::vec3 half_widths = b.get_half_widths();
  for(GLuint ii= 0; ii < 3; ii++) {
    GLfloat dist = glm::dot(d,axes[ii]);
    if(dist > half_widths[ii]) {
      dist = half_widths[ii];
    }
    if(dist < -half_widths[ii]) {
      dist = -half_widths[ii];
    }
    q+=dist*axes[ii];
  }
}

GLboolean CollisionDetectionAlgorithms::intersection_between(const BoundingSphere * bs, const BoundingOBB * obb,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) {
  glm::vec3 bs_center = glm::vec3(bs->get_center());
  glm::vec3 closest_point;
  closest_point_on_obb_to_point(bs_center,*obb,closest_point);
  GLfloat distance2 = glm::distance2(bs_center,closest_point);
  
  if(distance2 > std::pow(bs->get_radius(),2)) {
    return GL_FALSE;
  }
  if(calculate_collision_information) {
    *point_of_intersection = closest_point;
    *normal_of_intersection = -glm::normalize(bs_center - closest_point);
    *depth_of_intersection = bs->get_radius() - std::sqrt(distance2);
    return GL_TRUE;
  } else {
    return GL_TRUE;
  }
}

GLboolean CollisionDetectionAlgorithms::intersection_between(const BoundingOBB * a, const BoundingOBB * b,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) {
  // From RTCD p. 103
  // Compute rotation matrix from b's coordinates to a's coordinates
  glm::mat3 r,abs_r;
  GLfloat ra,rb;
  GLfloat epsilon = 0.0001;

  glm::mat3 axes_a = a->get_axes();
  glm::mat3 axes_b = b->get_axes();
  glm::vec3 half_widths_a = a->get_half_widths();
  glm::vec3 half_widths_b = b->get_half_widths();
  glm::vec3 center_a = glm::vec3(a->get_center());
  glm::vec3 center_b = glm::vec3(b->get_center());

  GLfloat min_penetration = std::numeric_limits<GLfloat>::max();
  GLint axis_index = 0;
  GLint penetration_index;

  for(GLuint ii= 0; ii < 3; ii++) {
    for(GLuint jj=0; jj < 3; jj++) {
      r[ii][jj] = glm::dot(axes_a[ii],axes_b[jj]);
    }
  }
  // And then translation
  glm::vec3 t = center_b - center_a;
  // Translation in a's coordinates
  t = glm::vec3(glm::dot(t,axes_a[0]),glm::dot(t,axes_a[1]),glm::dot(t,axes_a[2]));

  // Add a small value to make method robust when the two edges are parallel.
  for(GLuint ii=0; ii<3;ii++) {
    for(GLuint jj=0;jj<3;jj++) {
      abs_r[ii][jj] = std::abs(r[ii][jj]) + epsilon;
    }
  }

  for(GLuint ii=0; ii<3;ii++) {
    ra = half_widths_a[ii];
    rb = half_widths_b[0]*abs_r[ii][0] + half_widths_b[1]*abs_r[ii][1] + half_widths_b[2]*abs_r[ii][2];
    GLfloat lhs = std::abs(t[ii]);
    if(lhs > ra + rb) {
      return GL_FALSE;
    } else {
      GLfloat penetration = (ra + rb) - lhs;
      if(penetration < min_penetration) {
	min_penetration = penetration;
	penetration_index = axis_index;
      }
    }
    axis_index++;
  }
  
  for(GLuint ii= 0; ii < 3; ii++) {
    ra = half_widths_a[0]*abs_r[0][ii] + half_widths_a[1] * abs_r[1][ii] + half_widths_a[2]*abs_r[2][ii];
    rb = half_widths_b[ii];
    GLfloat lhs = std::abs(t[0]*r[0][ii] + t[1]*r[1][ii] + t[2]*r[2][ii]);
    if(lhs > ra + rb) {
      return GL_FALSE;
    } else {
      GLfloat penetration = (ra + rb) - lhs;
      if(penetration < min_penetration) {
	min_penetration = penetration;
	penetration_index = axis_index;
      }
    }
    axis_index++;
  }

  // L = A0xB1
  ra = half_widths_a[1]*abs_r[2][0] + half_widths_a[2]*abs_r[1][0];
  rb = half_widths_b[1]*abs_r[0][2] + half_widths_b[2]*abs_r[0][1];
  GLfloat lhs =std::abs(t[2]*r[1][0] - t[1]*r[2][0]);
  if(lhs > ra + rb) {
    return GL_FALSE;
  } /*else {
    
    GLfloat penetration = (ra + rb) - lhs;
    if(penetration < min_penetration) {
      min_penetration = penetration;
      penetration_index = axis_index;
    }
  }
    */
  axis_index++;
  // L = A0xB2
  ra = half_widths_a[1]*abs_r[2][2] + half_widths_a[2]*abs_r[1][2];
  rb = half_widths_b[0]*abs_r[0][1] + half_widths_b[1]*abs_r[0][0];
  lhs = std::abs(t[2]*r[1][2] - t[1]*r[2][2]);
  if(lhs > ra + rb) {
    return GL_FALSE;
  } /*else {
    GLfloat penetration = (ra + rb) - lhs;
    if(penetration < min_penetration) {
      min_penetration = penetration;
      penetration_index = axis_index;
    }
    }*/
  axis_index++;
  // L = A1xB0
  ra = half_widths_a[0]*abs_r[2][0] + half_widths_a[2]*abs_r[0][0];
  rb = half_widths_b[1]*abs_r[1][2] + half_widths_b[2]*abs_r[1][1];
  lhs = std::abs(t[0]*r[2][1] - t[2]*r[0][1]);
  if(lhs > ra + rb) {
    return GL_FALSE;
  } /*else {
    GLfloat penetration = (ra + rb) - lhs;
    if(penetration < min_penetration) {
      min_penetration = penetration;
      penetration_index = axis_index;
    }
    }*/
  axis_index++;

  // L = A1xB2
  ra = half_widths_a[0]*abs_r[2][2] + half_widths_a[2]*abs_r[0][2];
  rb = half_widths_b[0]*abs_r[1][1] + half_widths_b[1]*abs_r[1][0];
  lhs = std::abs(t[0]*r[2][2] - t[2]*r[0][2]);
  if(lhs > ra + rb) {
    return GL_FALSE;
  } /*else {
    GLfloat penetration = (ra + rb) - lhs;
    if(penetration < min_penetration) {
      min_penetration = penetration;
      penetration_index = axis_index;
    }
    }*/
  axis_index++;
  // L = A2xB0
  ra = half_widths_a[0]*abs_r[1][0] + half_widths_a[1]*abs_r[0][0];
  rb = half_widths_b[1]*abs_r[2][2] + half_widths_b[2]*abs_r[2][1];
  lhs = std::abs(t[1]*r[0][0] - t[0]*r[1][0]);
  if(lhs > ra + rb) {
    return GL_FALSE;
  } /*else {
    GLfloat penetration = (ra + rb) - lhs;
    if(penetration < min_penetration) {
      min_penetration = penetration;
      penetration_index = axis_index;
    }
    }*/
  axis_index++;

  // L = A2xB1
  ra = half_widths_a[0]*abs_r[1][1] + half_widths_a[1]*abs_r[0][1];
  rb = half_widths_b[0]*abs_r[2][2] + half_widths_b[2]*abs_r[2][0];
  lhs = std::abs(t[1]*r[0][1] - t[0]*r[1][1]);
  if(lhs > ra + rb) {
    return GL_FALSE;
  } /*else {
    GLfloat penetration = (ra + rb) - lhs;
    if(penetration < min_penetration) {
      min_penetration = penetration;
      penetration_index = axis_index;
    }
    }*/
  axis_index++;

  // L = A2xB2
  ra = half_widths_a[0]*abs_r[1][2] + half_widths_a[1]*abs_r[0][2];
  rb = half_widths_b[0]*abs_r[2][1] + half_widths_b[1]*abs_r[2][0];
  lhs = std::abs(t[1]*r[0][2] - t[0]*r[1][2]);
  if(lhs > ra + rb) {
    return GL_FALSE;
  } /*else {
    GLfloat penetration = (ra + rb) - lhs;
    if(penetration < min_penetration) {
      min_penetration = penetration;
      penetration_index = axis_index;
    }
    }*/
  axis_index++;

  if(calculate_collision_information) {
    
    GLfloat intersection_plane_constant;
    // Normal of intersection must be one of the axes of either a or b. To choose which one, look for the separating axis (only include separating axes parallel to obb axes) which has the smallest overlap. Choose either obb a or b based on that.
    if(penetration_index < 3) {
      *normal_of_intersection = axes_a[penetration_index];
      if(glm::dot(center_b - center_a,*normal_of_intersection) < 0.0f) {
	*normal_of_intersection = -1.0f*(*normal_of_intersection);
      }
      intersection_plane_constant = glm::dot(center_a + *normal_of_intersection*half_widths_a[penetration_index],*normal_of_intersection);
    } else {
      *normal_of_intersection = axes_b[penetration_index-3];
      if(glm::dot(center_a - center_b,*normal_of_intersection) < 0.0f) {
	*normal_of_intersection = -1.0f*(*normal_of_intersection);
      }
      intersection_plane_constant = glm::dot(center_b + *normal_of_intersection*half_widths_b[penetration_index-3],*normal_of_intersection);
    }
    // Find the point of intersection by looking for all the vertices of b (a), and checking whether they are closer to the center of a (b) than the minumal penetration separating plane.
    std::vector<glm::vec3> vertices;
    std::vector<GLfloat> projected_vertices;
    if(penetration_index < 3) {
      b->get_vertices(vertices);
    } else {
      a->get_vertices(vertices);
    }
    for(GLuint ii=0; ii<vertices.size();ii++) {
      projected_vertices.push_back(glm::dot(vertices[ii],*normal_of_intersection) - intersection_plane_constant-epsilon);
    }
    glm::vec3 cumulated_point_of_intersection = glm::vec3(0,0,0);
    GLuint nbr_included_vertices = 0;
    for(GLuint ii=0; ii < vertices.size(); ii++) {
      if(projected_vertices[ii]<=0.0f) {
	cumulated_point_of_intersection += vertices[ii];
	nbr_included_vertices++;
      }
    }
    if(nbr_included_vertices == 0) {
      // We chose the incorrect axis! //TODO better way to handle this!
      return GL_FALSE;
    }
    assert(nbr_included_vertices); // There must be an intersection
    if(nbr_included_vertices ==4) { // Face-to-face collision
      std::vector<glm::vec3> vertices2;
      std::vector<GLfloat> projected_vertices2;
      if(penetration_index < 3) {
	a->get_vertices(vertices2);
      } else {
	b->get_vertices(vertices2);
      }
      for(GLuint ii=0; ii<vertices2.size();ii++) {
	projected_vertices2.push_back(-1*glm::dot(vertices2[ii],*normal_of_intersection) - intersection_plane_constant-epsilon);
      }
      for(GLuint ii=0; ii < vertices2.size(); ii++) {
	if(projected_vertices2[ii]<0.0f) {
	  cumulated_point_of_intersection += vertices2[ii];
	  nbr_included_vertices++;
	}
      }
    }
    *point_of_intersection = cumulated_point_of_intersection/static_cast<GLfloat>(nbr_included_vertices);
    
    if(penetration_index>=3) {
      *normal_of_intersection = -(*normal_of_intersection);
    }
    
    *depth_of_intersection = min_penetration;
    
    //std::cout << "True! Center a: " << center_a[0] << ", " << center_a[1] << ", " << center_a[2] << std::endl <<  "      Center b: " << center_b[0] << ", " << center_b[1] << ", " << center_b[2] << std::endl << "Point of intersection: " << (*point_of_intersection)[0] << ", " << (*point_of_intersection)[1] << ", " << (*point_of_intersection)[2] << "." << std::endl << "Normal of intersection: " << (*normal_of_intersection)[0] << ", " << (*normal_of_intersection)[1] << ", " << (*normal_of_intersection)[2] << ". Min penetration is " << min_penetration << ". Index: " << penetration_index <<" From collision_detection_algorithms.cpp." << std::endl;
    return GL_TRUE;
  } else {
    return GL_TRUE;
  }

}

/**
   This is copied from http://www.lighthouse3d.com/tutorials/maths/ray-triangle-intersection/, which uses Tomas Möllers article (can be found on http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf)
 **/

GLboolean CollisionDetectionAlgorithms::ray_triangle_intersection(const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec3 & v2,const glm::vec3 & p,const glm::vec3 & d) {
  glm::vec3 e1 = v1-v0;
  glm::vec3 e2 = v2-v0;
  glm::vec3 h = glm::cross(d,e2);
  GLfloat a = glm::dot(e1,h);
  if (a > -0.00001 && a < 0.00001) {
    return(GL_FALSE);
  }
  GLfloat f = 1.0f/a;
  glm::vec3 s = p - v0;
  GLfloat u = f*glm::dot(s,h);
  if (u < 0.0 || u > 1.0) {
    return(GL_FALSE);
  }
  glm::vec3 q = glm::cross(s,e1);
  GLfloat  v = f*glm::dot(d,q);
  if (v < 0.0 || u + v > 1.0) {
    return(GL_FALSE);
  }
  // at this stage we can compute t to find out where the intersection point is on the line
  GLfloat t = f * glm::dot(e2,q);
  if (t > 0.00001) {// ray intersection
    return(GL_TRUE);
  } else // this means that there is a line intersection but not a ray intersection
    return (GL_FALSE);
}
