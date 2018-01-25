#ifndef UNIFORM_CONTAINER_HPP
#define UNIFORM_CONTAINER_HPP

class Object;

class UniformContainer {
private:
  //std::unordered_set uniforms; // Maybe not needed. Must be hard-coded
public:
  UniformContainer() {}
  virtual void update_uniforms(Object * parent) {}
  virtual void pre_draw_function() {}
  virtual void post_draw_function() {}
  virtual ~UniformContainer() {}
};
#endif
