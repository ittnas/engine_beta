#ifndef FPS_TEXT_OBJECT_HPP 
#define FPS_TEXT_OBJECT_HPP

#include "text_object.hpp"
#include "fps_logger.hpp"

class SetTextAction : public Action {
private:
public:
  SetTextAction(GLint stop = -1, GLint start = -1);
  virtual ~SetTextAction() {}
  virtual void perform_action(Object * target, GLint time);
};

class FPSTextObject : virtual public TextObject {
private:
  FPSLogger * fps_logger;
  GLint previous_update;
protected:
  virtual void action_phase_function();
  virtual void post_render_function(GLint tick);
public:
  FPSTextObject();
  virtual ~FPSTextObject();
  void set_fps_logger(FPSLogger * fps_logger);
  virtual void set_to_default_text();

};

#endif
