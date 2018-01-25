#ifndef WORLD_HPP
#define WORLD_HPP

#include <forward_list>

#include "object.hpp"
#include "projection_buffer.hpp"
#include "camera.hpp"
#include "light_manager.hpp"
#include "manager.hpp"
#include "screen.hpp"
#include "physics_manager.hpp"

class World : virtual public Object {
private:
  GLfloat inf;
  ProjectionBuffer * projection_buffer;
  static std::forward_list<World *> worlds;
  std::forward_list<Camera *> cameras; //Cameras that are physically located in the wolrd
  std::forward_list<Camera *> screens; // Screens of cameras that show their content into this world. The screens have to be updated before the world can be drawn.
  std::forward_list<Manager *> drawing_managers;
  std::forward_list<Manager *> simulation_managers;
  LightManager * light_manager;
  //ObjectManager object_manager; // NOT IMPLEMENTED!
  PhysicsManager * physics_manager; // NOT IMPLEMENTED

  //void execute_drawing(GLint tick, GLuint render_flag = -1, GLuint current_program = 0, Comparator comp = OR);

public:
  World();
  virtual ~World();
  void set_infinity(GLfloat infinity) {this->inf = infinity;}
  void set_projection(glm::mat4 projection_matrix);
  void register_object(Object *);
  void deregister_object(Object *);
  void set_light_manager(LightManager *);
  void set_physcis_manager(PhysicsManager *);
  PhysicsManager * get_physics_manager() {return physics_manager;}
  LightManager * get_light_manager() {return light_manager;}
  void add_drawing_manager(Manager * manager);
  void add_simulation_manager(Manager * manager);
  // When registering, call all the buffers to allow them register the object. Maybe. Cameras need to be registered, too. THE CURRENT WAY IS FINE.
  void add_screen(Screen * screen);
  static void action_on_worlds(GLint tick,GLfloat dt);
  void draw_world(GLint tick, GLuint render_flag = -1, GLuint current_program = 0, Comparator comp = OR);
  void draw_screens(GLint tick, GLuint render_flag, GLuint current_program, Comparator comp);
  void set_time(GLfloat tick) {this->projection_buffer->set_time(tick);}
  void run_simulation_managers();
  void update_all_buffers();
};

#endif
