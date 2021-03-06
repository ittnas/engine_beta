#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include <set>

#include "action.hpp"
#include "program.hpp"
#include "shape.hpp"
#include "render_flag.hpp"

class Buffer;
class World;
class Drawer;
class UniformContainer;

class Object {
private:
  std::vector<Drawer *> drawers;
  std::unordered_map<std::string,Buffer *> buffers;
  std::vector<UniformContainer *> uniform_containers;
  std::vector<Program *> programs;
  Shape * shape;
  Object * parent;
  World * world;
  std::vector<Object *> children;
  std::vector<Action *> action_vector;
  //std::set<RenderFlag> render_flags;
  GLuint render_flag;
  GLuint physics_flag;

  void update_ubos_phase2();
  GLboolean skip_rendering(GLuint render_flag,Comparator comp);

protected:
  std::vector<Object *> get_children();
  GLuint get_program_ident(GLuint program_position = 0);
  void set_world(World * world);
  virtual void action_phase_function() {}
  virtual void post_render_function(GLint tick) {}
  void update_ubos_phase1();
  static GLboolean compare(GLuint state,GLuint mask,Comparator comp);
  GLboolean skip_physics(GLuint physics_mask,Comparator comp);
public:
  Object();
  virtual ~Object();
  void set_shape(Shape *);
  void set_program(Program * prog, GLuint program_position = 0);
  void set_parent(Object * parent);
  void add_child(Object * child);
  void replace_parent(Object * parent); // Remove this object from previous parents list of children
  void remove_child(Object *child);
  void clear_children();
  Shape * get_shape();
  Program * get_program(GLuint program_position = 0);
  Object * get_parent();

  void set_render_flag(GLuint flag) {this->render_flag = flag;}
  void add_render_flag(GLuint flag) {this->render_flag = this->render_flag | flag;}
  void remove_render_flag(GLuint flag) {this->render_flag = this->render_flag - (this->render_flag & flag);}
  void set_phyics_flag(GLuint flag) {this->physics_flag = flag;}
  void add_physics_flag(GLuint flag) {this->physics_flag = this->physics_flag | flag;}
  void remove_physics_flag(GLuint flag) {this->physics_flag = this->physics_flag - (this->render_flag & flag);}
  void set_render_flag_to_all(GLuint flag);
  void add_render_flag_to_all(GLuint flag);
  GLuint get_render_flag() const {return this->render_flag;}
  void remove_render_flag_from_all(GLuint flag);
  World * get_world();
  void render(GLint tick, GLuint render_flag = -1, GLuint current_program = 0, Comparator comp = OR, DrawingMode = NOTSET);
  void add_buffer(Buffer * buffer);
  void remove_buffer(std::string buffer_name);
  Buffer * get_buffer(std::string buffer_name);
  void add_uniform_container(UniformContainer * uniform_container);
  void perform_action(GLint tick);
  void add_action(Action * action);
  void print_children() const;
};

#endif
