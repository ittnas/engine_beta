#include <algorithm>

#include "object.hpp"
#include "buffer.hpp"
#include "drawer.hpp"
#include "world.hpp"
#include "uniform_list.hpp"
#include "uniform_container.hpp"

Object::Object() : shape(0), parent(0),render_flag(0), world(0) {
  
}

void Object::update_ubos_phase1() {
  for(auto buffer : buffers) {
    buffer.second->pre_update_function();
  }
  for(auto child : children) {
    child->update_ubos_phase1();
  }

  for(auto buffer : buffers) {
    buffer.second->update_buffer_phase1(this);
  }
  for(auto buffer : buffers) {
    buffer.second->post_update_function();
  }
}

void Object::update_ubos_phase2() {
  /*for(auto child : get_children()) {
    child->update_ubos_phase2();
  }
  */
  for(auto buffer : buffers) {
    buffer.second->update_buffer_phase2(this);
  }
  for(auto uniform_container : uniform_containers) {
    uniform_container->update_uniforms(this);
  }
}

GLboolean Object::skip_rendering(GLuint require_mask,Comparator comp) {
  /*
  switch (comp) {
  case AND: return !((require_mask & this->render_flag) == require_mask); break;
  case OR: return !((require_mask & this->render_flag)); break; //Require that at least one of the rendering modes is set.
  case NOT: return require_mask & this->render_flag; break;
  case ANY: return false; break;
  }
  //return render_flags.find(render_flag) == render_flags.end();
  */
  return !compare(this->render_flag,require_mask,comp);
}

std::vector<Object *> Object::get_children() {
  return children;
}

GLuint Object::get_program_ident(GLuint program_position) {
  get_program(program_position)->get_program_ident();
}

void Object::set_shape(Shape * shape) {
  this->shape = shape;
}

void Object::set_program(Program * prog, GLuint program_position) {
  if(programs.size() <= program_position) {
    programs.resize(program_position+1,0);
  }
  programs.at(program_position) = prog;
  /*if(prog!=0) {
    //Moved to program creation
    // Registers the program with all buffers. It is enough to do this once per program per buffer, but as setting the program should be quite rare, it is ok to do it for every new object.
    for(auto buffer : buffers) {
      prog->set_uniform_block_binding(buffer.second->get_ubo_name(),buffer.second->get_binding_point());
    }
    }*/
}


void Object::set_parent(Object * parent) {
  if(this->parent != 0) {
    this->parent->remove_child(this);
  }
  this->parent = parent;
  if(parent!=0) {
    parent->add_child(this);
  }
}

void Object::add_child(Object * child) {
  if(child !=0) {
    children.push_back(child);
    child->replace_parent(this);
    child->set_world(this->world);
    //this->print_children();
  }
}
void Object::replace_parent(Object * parent) {
  if(parent != 0) {
    if(this->parent != 0 && this->parent != parent) {
      this->parent->remove_child(this);
    }
    this->parent = parent;
  }
}

void Object::remove_child(Object * child) {
  auto new_vector = std::remove(children.begin(), children.end(),child);
  if(new_vector == children.end()) {
    // child not found.
  } else {
    child->set_world(0);
    child->replace_parent(0);
  }
    children.erase(new_vector,children.end());
}

void Object::clear_children() {
  for(auto child : children) {
    child->set_world(0);
    child->replace_parent(0);
  }

  //children.erase(children.begin(),children.end());
  children.clear();
  /*
  for(auto child : children) {
    remove_child(child);
  }
  */
}

Shape * Object::get_shape() {
  return shape;
}
Program * Object::get_program(GLuint program_position) {
  if(programs.size() > program_position && programs[program_position] != 0) {
    return programs[program_position]; //success!
  } else {
    if(parent != 0) {
      Program * prog = parent->get_program(program_position);
      set_program(prog);
      return prog;
    } else {
      return 0;
    }
  }
}

Object * Object::get_parent() {
  return parent;
}

void Object::render(GLint tick,GLuint render_flag,GLuint current_program,Comparator comp, DrawingMode drawing_mode) {
  for(auto buffer : buffers) {
    buffer.second->pre_draw_function();
  }
  for(auto uniform_container : uniform_containers) {
    uniform_container->pre_draw_function();
  }

  update_ubos_phase2();

  if(shape!=0 && !skip_rendering(render_flag,comp)) {
    glUseProgram(get_program_ident(current_program));
    UniformList::set_current_program_ident(get_program_ident(current_program));
    for(auto drawer : drawers) {
      drawer->pre_draw_function(this);
    }
    shape->draw(drawing_mode);
    glUseProgram(0);
    for(auto drawer : drawers) {
      drawer->post_draw_function(this);
    }
  }
  for(auto child : get_children()) {
    child->render(tick,render_flag,current_program,comp,drawing_mode);
  }
  for(auto buffer : buffers) {
      buffer.second->post_draw_function();
  }
  for(auto uniform_container : uniform_containers) {
    uniform_container->post_draw_function();
  }
  post_render_function(tick);
}

void Object::set_world(World * world) {
  if(world != 0) {
    world->register_object(this);
  }
  if(this->world != 0) {
    this->world->deregister_object(this);
  }
  this->world = world;

  for(auto child : get_children()) {
    child->set_world(world);
  }
}

World * Object::get_world() {
  return this->world;
}

void Object::add_buffer(Buffer * buffer) {
  auto existing_buffer = this->buffers.find(buffer->get_ubo_name());
  if(existing_buffer == buffers.end()) {
    // Not found
    this->buffers.insert(std::pair<std::string,Buffer *>(buffer->get_ubo_name(),buffer));
  } else {
    assert(0); //Buffer already exists.
  }
}

void Object::remove_buffer(std::string buffer_name) {
  buffers.erase(buffer_name);
}

Buffer * Object::get_buffer(std::string buffer_name) {
  auto iterator = buffers.find(buffer_name);
  if(iterator == buffers.end()) {
    return 0;
  } else {
    return iterator->second;
  }
}

Object::~Object() {
  if(parent != 0) {
    this->set_parent(0);
  }
  for(auto child : children) {
    //child->set_parent(0);
    delete child;
  }
}

void Object::perform_action(GLint tick) {
  
  for(auto child : get_children()) {
    child->perform_action(tick);
  }
  action_phase_function();
  for(auto action : action_vector) {
    action->perform_action(this,tick);
  }
  //update_ubos_phase1();
}

void Object::add_action(Action * action) {
  action_vector.push_back(action);
}

void Object::add_uniform_container(UniformContainer * uniform_container) {
  uniform_containers.push_back(uniform_container);
}

void Object::set_render_flag_to_all(GLuint flag) {
  for(auto child : children) {
    child->set_render_flag_to_all(flag);
  }
  set_render_flag(flag);
}

void Object::add_render_flag_to_all(GLuint flag) {
  for(auto child : children) {
    child->add_render_flag_to_all(flag);
  }
  add_render_flag(flag);
}

void Object::remove_render_flag_from_all(GLuint flag) {
  for(auto child : children) {
    child->remove_render_flag_from_all(flag);
  }
  remove_render_flag(flag);
}

/**
   Compares the mask to the state of the object. If the mask matches the state, as given by the comparator, returns true. Otherwise returns false
**/

GLboolean Object::compare(GLuint state,GLuint mask,Comparator comp) {
  switch (comp) {
  case AND: return ((mask & state) == mask); break;
  case OR: return (mask & state); break; //Require that at least one of the rendering modes is set.
  case NOT: return !(mask & state); break;
  case ANY: return true; break;
  }
}

GLboolean Object::skip_physics(GLuint physics_mask,Comparator comp) {
  return !compare(this->physics_flag,physics_mask,comp);
}

void Object::print_children() const {
  std::cout << "Object " << this << " Has the following children:" << std::endl;
  for(auto child : children) {
    std::cout << child << std::endl;
  }
}
