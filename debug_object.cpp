#include <algorithm>

#include "object.hpp"
#include "buffer.hpp"
#include "drawer.hpp"
//#include "world.hpp"

Object::Object() : shape(0), parent(0),render_flag(-1) {
  
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
}

GLboolean Object::skip_rendering(GLuint require_mask,Comparator comp) {
  switch (comp) {
  case AND: return !((require_mask & this->render_flag) == require_mask); break;
  case OR: return !((render_flag & this->render_flag)); break; //Require that at least one of the rendering modes is set.
  }
  //return render_flags.find(render_flag) == render_flags.end();
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
    children.push_back(child);child->replace_parent(this);
    //child->set_world(this->world);
  }
}
void Object::replace_parent(Object * parent) {
  if(parent != 0) {
    if(this->parent != 0) {
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
    //child->set_world(0);
  }
    children.erase(new_vector,children.end());
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

void Object::render(GLint tick,GLuint render_flag,GLuint current_program,Comparator comp) {
  for(auto buffer : buffers) {
    buffer.second->pre_draw_function();
  }
  if(shape!=0 && !skip_rendering(render_flag,comp)) {
    update_ubos_phase2();
    glUseProgram(get_program_ident(current_program));
    for(auto drawer : drawers) {
      drawer->pre_draw_function(this);
    }
    shape->draw();
    glUseProgram(0);
    for(auto drawer : drawers) {
      drawer->post_draw_function(this);
    }
  }
  for(auto child : get_children()) {
    child->render(tick,render_flag,current_program);
  }
  for(auto buffer : buffers) {
      buffer.second->post_draw_function();
    }

}
/*
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
*/
/*
World * Object::get_world() {
  return this->world;
}
*/
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

