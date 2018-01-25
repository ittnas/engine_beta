#ifndef FORCE_HPP 
#define FORCE_HPP
#define GLM_FORCE_RADIANS
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "movable.hpp"

enum ForceType {
  GRAVITY,ELECTROMAGNETIC,LORENZ
};
class Force {
private:
  ForceType force_type;
  virtual glm::vec3 get_strength_sub_class(const glm::vec3 & position) const = 0;
protected:
  Force(ForceType type);
public:
  glm::vec3 get_strength(const glm::vec3 & position) const;
  ForceType get_type() const {return force_type;}
  virtual ~Force() {}
};

class ConstantForce : public Force {
private:
  virtual glm::vec3 get_strength_sub_class(const glm::vec3 & position) const;
  glm::vec3 force_vector;
public:
  ConstantForce(ForceType type,glm::vec3 force_vector);
  virtual ~ConstantForce() {}
};

class PointForce : public Force {
private:
  virtual glm::vec3 get_strength_sub_class(const glm::vec3 & position) const;
  glm::vec3 force_position;
  GLfloat field_strength;
  GLfloat max_strength;
public:
  PointForce(ForceType type,glm::vec3 position, GLfloat field_strength,GLfloat max_strength = std::numeric_limits<GLfloat>::max());
  void set_max_strength(GLfloat max_strength) {this->max_strength = max_strength;}
  virtual ~PointForce() {}
};

class CenterOfMassForce : public Force {
private:
  const Movable * object;
  GLfloat field_strength;
  virtual glm::vec3 get_strength_sub_class(const glm::vec3 & position) const;
public:
  CenterOfMassForce(ForceType type, const Movable * object,GLfloat field_strength);
  ~CenterOfMassForce() {}
};

#endif
