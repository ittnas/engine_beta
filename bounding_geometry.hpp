#ifndef BOUNDING_GEOMETRY_HPP
#define BOUNDING_GEOMETRY_HPP

#include <vector>
//#include <tuple>

#include "collision_object.hpp"

class BoundingSphere;
class BoundingAABB;
class BoundingOBB;

class BoundingGeometry {
private:
  /*
  std::vector<glm::vec4> mesh; // Move to CollisionObject
  CollisionObject * target_object;
  GLuint position_array_index;
  */
  
protected:
  /*
  virtual void translate_geometry(glm::vec3 translation) = 0;
  virtual void rotate_geometry(glm::fquat orientation) = 0;
  virtual void scale_geometry(GLfloat scale_factor) = 0;*/
  //BoundingGeometry(CollisionObject * target_object,GLuint position_array_index = 0);
  BoundingGeometry() {}
public:
  virtual ~BoundingGeometry() {}
  virtual void transform_geometry(const glm::vec4 &,const glm::fquat &,GLfloat,const glm::vec3 &) = 0;
  //void recalculate_mesh(); // Here should be somehow implented the inclusion of only colliding parts... NOT NEEDED HERE
  //virtual std::tuple<GLboolean,glm::vec3,glm::vec3,GLfloat> intersect(const BoundingSphere *,GLboolean calculate_collision_information) const= 0;
  virtual GLboolean intersect(const BoundingSphere *,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) const= 0;
  virtual GLboolean intersect(const BoundingAABB *,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) const= 0;
  virtual GLboolean intersect(const BoundingOBB *,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) const= 0;
  //virtual GLboolean intersect(const BoundingOBB *, GLboolean calculate_collision_information,glm::vec3 * point_of_intersection, glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection) const = 0;
  // virtual std::tuple<GLboolean,glm::vec3,glm::vec3,GLfloat> intersect(const BoundingAABB *,GLboolean calculate_collision_information) const= 0; //etc...
  //virtual std::tuple<GLboolean,glm::vec3,glm::vec3,GLfloat> check_intersection(const BoundingGeometry &,GLboolean calculate_collision_information) = 0; // accept of double dispatch
  virtual GLboolean check_intersection(const BoundingGeometry &,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) = 0; // accept of double dispatch
  //static BoundingGeometry * create_from(BoundingBox * box); // This should be in the factory.
  virtual Shape * create_shape() const = 0;
  static glm::mat4 get_transformation_matrix(const glm::vec4 &,const glm::fquat &,const glm::vec3 &,const glm::vec3 & = glm::vec3(0,0,0));
};

class BoundingSphere : public BoundingGeometry {
private:
  glm::vec4 center;
  GLfloat radius;
  glm::vec4 transformed_center;
  GLfloat transformed_radius;
public: 
  //BoundingSphere(CollisionObject * target_object,GLuint position_array_index = 0);
  BoundingSphere(glm::vec4 center,GLfloat radius);
  virtual ~BoundingSphere() {}
  void transform_geometry(const glm::vec4 &,const glm::fquat &,GLfloat,const glm::vec3 &);
  //std::tuple<GLboolean,glm::vec3,glm::vec3,GLfloat> intersect(const BoundingSphere *,GLboolean calculate_collision_information) const;
  //std::tuple<GLboolean,glm::vec3,glm::vec3,GLfloat> intersect(const BoundingAABB *,GLboolean calculate_collision_information) const;
  GLboolean intersect(const BoundingSphere *,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) const;
  GLboolean intersect(const BoundingAABB *,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) const;
  GLboolean intersect(const BoundingOBB *,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) const;
  glm::vec4 get_center() const {return transformed_center;}
  GLfloat get_radius() const {return transformed_radius;}
  Shape * create_shape() const;
  GLboolean check_intersection(const BoundingGeometry &,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ); // accept of double dispatch
  //std::tuple<GLboolean,glm::vec3,glm::vec3,GLfloat> check_intersection(const BoundingGeometry &,GLboolean calculate_collision_information);
};

class BoundingAABB : public BoundingGeometry {
private:
  glm::vec3 min;
  glm::vec3 max;
  glm::vec3 trans_min;
  glm::vec3 trans_max;
public:
  BoundingAABB(glm::vec3 min,glm::vec3 max);
  virtual ~BoundingAABB() {}
  void transform_geometry(const glm::vec4 &,const glm::fquat &,GLfloat,const glm::vec3 &);
  GLboolean intersect(const BoundingSphere *,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) const;
  GLboolean intersect(const BoundingAABB *,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) const;
  GLboolean intersect(const BoundingOBB *,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) const;
  GLboolean check_intersection(const BoundingGeometry &,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ); // accept of double dispatch
  glm::vec3 get_trans_min() const {return trans_min;}
  glm::vec3 get_trans_max() const {return trans_max;}
  Shape * create_shape() const;
};

class BoundingOBB : public BoundingGeometry {
private:
  glm::vec4 center;
  //glm::fquat orientation; // Do not use quaternion, as they need to cast to rotation matrix every frame
  glm::vec3 half_widths;
  glm::mat3 axes;
  glm::vec4 transformed_center;
  glm::vec3 transformed_half_widths;
  glm::mat3 transformed_axes;
  
public:
  BoundingOBB(glm::vec4 center, glm::vec3 half_widths, glm::mat3 axes);
  BoundingOBB(const BoundingAABB & source);
  virtual ~BoundingOBB() {}
  GLboolean intersect(const BoundingSphere *,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) const;
  GLboolean intersect(const BoundingAABB *,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) const;
  GLboolean intersect(const BoundingOBB *,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ) const;
  GLboolean check_intersection(const BoundingGeometry &,GLboolean calculate_collision_information,glm::vec3 * point_of_intersection,glm::vec3 * normal_of_intersection, GLfloat * depth_of_intersection ); // accept of double dispatch
  void transform_geometry(const glm::vec4 &,const glm::fquat &,GLfloat,const glm::vec3 &);
  Shape * create_shape() const;
  glm::vec4 get_center() const {return transformed_center;}
  //glm::vec4 get_transformed_center() const {return transformed_center;}
  glm::mat3 get_axes() const {return transformed_axes;}
  //glm::mat3 get_transformed_axes() const {return transformed_axes;}
  glm::vec3 get_half_widths() const {return transformed_half_widths;}
  //glm::vec3 get_transformed_half_widths() const {return transformed_half_widths;}
  void get_vertices(std::vector<glm::vec3> & vertices) const;
};

class BoundingPlane : public BoundingGeometry {

};

class BoundingCylinder : public BoundingGeometry {
  
};

class BoundingEllipsoid : public BoundingGeometry {

};
#endif
