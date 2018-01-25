#include "world.hpp"
#include "object.hpp"
#include "projection_buffer.hpp"
#include "update_time_action.hpp"

std::forward_list<World*> World::worlds;

World::World() : Object(), inf(100.0), light_manager(0), physics_manager(0) {
  worlds.push_front(this);
  this->set_world(this);
  try {
    this->projection_buffer = new ProjectionBuffer();
  } catch (std::bad_alloc&) {
    std::cerr << "Memory allocation failed." << std::endl;
    assert(0);
  }
  this->add_buffer(this->projection_buffer);
  
  try {
    LightManager * lm = new LightManager();
    set_light_manager(lm);
    //this->add_buffer(lm);
    //this->add_manager(lm);
  } catch (std::bad_alloc&) {
    std::cerr << "Memory allocation failed." << std::endl;
    assert(0);
  }
  this->add_action(new UpdateTimeAction(-1,-1));
}

/*
  There is a risk of memory leak!
 */

void World::set_light_manager(LightManager * lm) {
  if(lm != NULL) {
    add_drawing_manager(lm);
    add_buffer(lm);
    this->light_manager = lm;
  } else {
    light_manager = 0;
  }
}

void World::set_physcis_manager(PhysicsManager * pm) {
  if(pm != NULL) {
    add_simulation_manager(pm);
    this->physics_manager = pm;
  } else {
    physics_manager = 0;
  }
}

void World::set_projection(glm::mat4 projection_matrix) {
  //ProjectionBuffer * buffer = dynamic_cast<ProjectionBuffer *>(get_buffer("projection_buffer"));
  //if(buffer == 0) {
    //Cast failed, i.e. there is no projection buffer
  //    assert(0);
  //}
  this->projection_buffer->set_projection(projection_matrix);
}

World::~World() {
  worlds.remove(this);
}

void World::action_on_worlds(GLint tick, GLfloat dt) {
  for(auto world : worlds) {
    
    if(world->get_physics_manager() != 0) {
      world->get_physics_manager()->apply_forces(dt);
      world->get_physics_manager()->update_motion(tick);
    }
    world->perform_action(tick);
    world->run_simulation_managers();
  }
}

void World::draw_screens(GLint tick, GLuint render_flag, GLuint current_program, Comparator comp) {
  for(auto screen : screens) {
    if(screen->not_too_many_invocations()) {
      screen->increment_invocations();
      screen->draw_view(tick,render_flag,current_program,comp);
    }
  }
}

void World::update_all_buffers() {
  for(auto manager : drawing_managers) {
    manager->manage();
  }
  update_ubos_phase1();

}
/*
void World::draw_world(GLint tick, GLuint render_flag, GLuint current_program, Comparator comp) {
  render(tick,render_flag,current_program,comp);
}
*/
/*
void World::execute_drawing(GLint tick, GLuint render_flag, GLuint current_program, Comparator comp) {
  update_ubos_phase1();
  render(tick,render_flag,current_program,comp);
  }*/

void World::add_drawing_manager(Manager * manager) {
  this->drawing_managers.push_front(manager);
}

void World::add_simulation_manager(Manager * manager) {
  this->simulation_managers.push_front(manager);
}

void World::register_object(Object * object) {
  for(auto manager : simulation_managers) {
    manager->register_object(object);
  }
  for(auto manager : drawing_managers) {
    manager->register_object(object);
  }
}

void World::deregister_object(Object * object) {
  for(auto manager : simulation_managers) {
    manager->deregister_object(object);
  }
  for(auto manager : drawing_managers) {
    manager->deregister_object(object);
  }
}

void World::run_simulation_managers() {
  for(auto manager : simulation_managers) {
    manager->manage();
  }
}
