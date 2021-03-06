#include "key_action.hpp"
#include "key_table.hpp"
#include "flying_camera.hpp"

KeyAction::KeyAction(GLint stop, GLint start) : Action(stop,start), speed(50),roll_speed(1000.0), pitch_speed(1000.0),previous_tick(-1),yaw_speed(1000.0) {
  key_table = new KeyTable();
}

void KeyAction::perform_action(Object * target, GLint time) {
  if(previous_tick == -1) {
    previous_tick = time;
  } else {
    FlyingCamera * cam = dynamic_cast<FlyingCamera *>(target);
    GLdouble passed_time = static_cast<GLdouble>(time - previous_tick) / 1000;
    // std::cout << passed_time << std::endl;
    
    if(key_table->get_status(SDLK_w)) {
      GLfloat distance = speed*passed_time;
      cam->move_forward(distance);
    }
    if(key_table->get_status(SDLK_s)) {
      GLfloat distance = speed*passed_time;
      cam->move_forward(-distance);
    }
    if(key_table->get_status(SDLK_a)) {
      GLfloat roll_angle = roll_speed*passed_time;
      cam->roll(roll_angle);
      //std::cout << roll_angle << std::endl;
    }
    if(key_table->get_status(SDLK_d)) {
      GLfloat roll_angle = roll_speed*passed_time;
      cam->roll(-roll_angle);
    }
    if(key_table->get_status(SDLK_UP)) {
      GLfloat pitch_angle = pitch_speed*passed_time;
      cam->pitch(-pitch_angle);
    }
    if(key_table->get_status(SDLK_DOWN)) {
      GLfloat pitch_angle = pitch_speed*passed_time;
      cam->pitch(pitch_angle);
    }
    if(key_table->get_status(SDLK_LEFT)) {
      GLfloat yaw_angle = yaw_speed*passed_time;
       cam->yaw(-yaw_angle);
    }
    if(key_table->get_status(SDLK_RIGHT)) {
      GLfloat yaw_angle = yaw_speed*passed_time;
       cam->yaw(yaw_angle);
    }
  }
  previous_tick = time;
}
