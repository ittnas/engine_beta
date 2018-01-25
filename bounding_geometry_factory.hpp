#ifndef BOUNDING_GEOMETRY_FACTORY_HPP
#define BOUNDING_GEOMETRY_FACTORY_HPP

#include "bounding_geometry.hpp"
#include "bounding_geometry_generation_algorithms.hpp"

typedef BoundingSphere * (*BoundingSphereFromAabbAlgorithm)(const BoundingAABB &); // Typedef should be in algorithms.h
typedef BoundingSphere * (*BoundingSphereFromSphereAlgorithm)(const BoundingSphere &);
typedef BoundingSphere * (*BoundingSphereFromObbAlgorithm)(const BoundingOBB &);
typedef BoundingSphere * (*BoundingSphereFromPointsAlgorithm)(const std::vector<glm::vec4> &);

class BoundingGeometryFactory {
private:
  BoundingSphereFromAabbAlgorithm bsfaabba;
  BoundingSphereFromObbAlgorithm bsfobba;
  BoundingSphereFromSphereAlgorithm bsfsa;
public:
  BoundingGeometryFactory();
  virtual ~BoundingGeometryFactory() {}
  //BoundingGeometry * generate_bounding_geometry(BoundingGeometryType bvt, Object * object);
protected:
  //virtual BoundingSphere * 
public:
  BoundingSphere * create_bounding_sphere_from(const BoundingAABB * box); // Maybe even from box, which bounding box then implements.
  BoundingAABB * create_bounding_aabb_from(const BoundingOBB * obb);
  //BoundingGeometry * create_bounding_geometry(BoundingGeometryType bgt);
public:
  void set_algorithm_bounding_sphere_from_aabb_box(BoundingSphereFromAabbAlgorithm bsfba);
  void set_algorithm_bounding_sphere_from_obb_bounding_box(BoundingSphereFromObbAlgorithm bsfba);
  void set_algorithm_bounding_sphere_from_bounding_sphere(BoundingSphereFromSphereAlgorithm bsfba);
};

#endif
