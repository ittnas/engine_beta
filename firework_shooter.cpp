#include <glm/glm.hpp>
#include <algorithm>

#include "firework_shooter.hpp"
#include "firework_shape.hpp"
#include "firework.hpp"
#include "firework_action.hpp"
#include "killer_action.hpp"

FireworkShooter::FireworkShooter() : Object(), Movable(), current_index(0) {
  //for(GLuint i= 0; i < n; i++) {
  //fireworks.push_back(new FireworkShape(1000,0.8,0.2,glm::vec3(0.8,0,0),glm::vec3(0.1,0.1,0.1)));
    //}
  
  fireworks.push_back(new FireworkShape(500,4.4,2.1,glm::vec3(0.8,0,0),glm::vec3(0.1,0.1,0.1)));
  
  fireworks.push_back(new FireworkShape(1500,6.8,4.2,glm::vec3(0.0,0.9,0.8),glm::vec3(0.1,0.1,0.1)));
  fireworks.push_back(new FireworkShape(500,4.0,2.5,glm::vec3(0.0,0.0,0.8),glm::vec3(0.0,0.0,0.3)));
  fireworks.push_back(new FireworkShape(1500,6.0,3.2,glm::vec3(0.4,0.0,0.8),glm::vec3(0.1,0.0,0.2)));
  fireworks.push_back(new FireworkShape(1500,5.0,4.2,glm::vec3(0.8,0.5,0.0),glm::vec3(0.2,0.2,0.0)));
  fireworks.push_back(new FireworkShape(5500,10.0,7.2,glm::vec3(0.7,0.0,0.8),glm::vec3(0.2,0.0,0.2)));
  
  n = fireworks.size();
    
  this->add_action(new FireworkAction(-1,-1));
  this->fwprog = new Program("firework.vert","firework.frag");
  this->killer_action = new KillerAction(10000,-1,-1);
}

GLvoid FireworkShooter::fire_next(GLfloat time) {
  Firework * fw = new Firework();
  fw->set_shape(fireworks[rand() % n]);
  //current_index++;
  GLfloat x = 2*3*((GLfloat)rand()/(RAND_MAX)-0.5);
  GLfloat z = 2*3*((GLfloat)rand()/(RAND_MAX)-0.5);

  GLfloat sx = 2*0.5*((GLfloat)rand()/(RAND_MAX)-0.5);
  GLfloat sz = 2*0.5*((GLfloat)rand()/(RAND_MAX)-0.5);
  GLfloat sy = 5*((GLfloat)rand()/(RAND_MAX) + 1.0);

  fw->set_position(glm::vec4(x,0,z,1));
  fw->set_initial_velocity(glm::vec3(sx,sy,sz));
  fw->set_creation_time(time);
  fw->set_explosion_time(time + 1.5);
  fw->set_program(fwprog);
  fw->add_action(killer_action);
  this->add_child(fw);
}
