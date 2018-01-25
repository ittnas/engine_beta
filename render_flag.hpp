#ifndef RENDER_FLAG_HPP
#define RENDER_FLAG_HPP

enum RenderFlag {
  SHADOWABLE = 1,
  SHADOWCASTER = 2,
  LIGHTABLE = 4,
  TRANSPARENT = 8
};

enum PhysicsFlag {
  COLLIDABLE = 1,
  COLLIDER = 2,
  DEFORMABLE = 4,
  STATIC = 8
};

enum Comparator {
  AND,
  OR,
  NOT,
  ANY
};

/*enum BoundingVolumeType {
  SPHERE = 1,
  AABB = 2,
  CYLINDER = 4
  };*/

#endif
