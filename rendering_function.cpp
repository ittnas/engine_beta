#include "rendering_function.hpp"
#include "light_manager.hpp"
#include "camera.hpp"
#include "world.hpp"

void RenderingFunction::set_target_camera(Camera * camera) {
  if(camera != 0) {
    target_camera = camera;
    target_world = camera->get_world();
  }
}

void OITRenderingFunction::perform_rendering(GLint tick, GLuint render_flag, GLuint current_program, Comparator comp,DrawingMode drawing_mode) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  this->target_camera->select_render_target();
  this->target_camera->draw(tick,TRANSPARENT,current_program,NOT);
  this->target_camera->update_world(); // Addition of this line prevents the flickering of the scene. Apparently some values are lost from the shaders (maybe?) after the rendering pass and they need to be releaded for the next one.
  this->target_camera->draw(tick,TRANSPARENT,current_program,AND,drawing_mode);
}

void ShadowVolumesRenderingFunction::perform_rendering(GLint tick, GLuint render_flag, GLuint current_program, Comparator comp,DrawingMode drawing_mode) {
  this->target_camera->select_render_target();

  glDisable(GL_BLEND);  
  glEnable(GL_DEPTH_TEST); // Activate the depth test
  glDepthMask(GL_TRUE);  // Writing on z-buffer
  glDepthFunc(GL_LESS); // We change the z-testing function to LESS, to avoid little bugs in shadow
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  this->target_camera->draw(tick,SHADOWABLE|SHADOWCASTER,current_program,NOT,INDEXED);
  LightManager * lm = this->target_camera->get_world()->get_light_manager();
  auto lights = lm->get_lights();
  for(auto & light : lights) {
    light.first->disable();
  }
  this->target_camera->update_world(); // Addition of this line prevents the flickering of // TODO: he scene. Apparently some values are lost from the shaders (maybe?) after the rendering pass and they need to be releaded for the next one.
  //this->target_camera->draw(tick,SHADOWABLE|SHADOWCASTER,0,OR,ADJACENCY);
  this->target_camera->draw(tick,SHADOWABLE|SHADOWCASTER,0,OR,INDEXED);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE); // The blending function scr+dst, to add all the lighting
  glDepthMask(GL_FALSE);  // We stop writing to z-buffer now. We made this in the first pass, now we have it
  //glDepthFunc(GL_LEQUAL); // we put it again to LESS or EQUAL (or else you will get some z-fighting)
  glDepthFunc(GL_LESS);
  glEnable(GL_STENCIL_TEST);

  for(auto & light : lights) {
    light.first->enable();
    glClear(GL_STENCIL_BUFFER_BIT);
    glColorMask(0,0,0,0);
    //glColorMask(1,1,1,1);
    glStencilFunc(GL_ALWAYS, 0, ~0);
    glStencilMask(~0);
    //glActiveStencilFaceEXT(GL_FRONT);
    glStencilOpSeparate(GL_FRONT,GL_KEEP, GL_DECR_WRAP, GL_KEEP);
    glStencilOpSeparate(GL_BACK,GL_KEEP, GL_INCR_WRAP, GL_KEEP);
    //glCullFace(GL_NONE);
    glDisable(GL_CULL_FACE);
    this->target_camera->update_world(); // Addition of this line prevents the flickering of The scene. Apparently some values are lost from the shaders (maybe?) after the rendering pass and they need to be reloaded for the next one.
    this->target_camera->draw(tick,SHADOWABLE|SHADOWCASTER,1,OR,ADJACENCY);

    glStencilFunc(GL_EQUAL, 0, ~0);
    //glActiveStencilFaceEXT(GL_FRONT);
    glStencilOpSeparate(GL_FRONT,GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilOpSeparate(GL_BACK,GL_KEEP, GL_KEEP, GL_KEEP);
    //glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glDepthFunc(GL_EQUAL);
    glColorMask(1, 1, 1, 1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    this->target_camera->update_world();
    this->target_camera->draw(tick,SHADOWABLE|SHADOWCASTER,2,OR,INDEXED);
    //this->target_camera->draw(tick,SHADOWABLE|SHADOWCASTER,0,OR,INDEXED);
    light.first->disable();
    break;
  }
  //this->target_camera->update_world(); // Addition of this line prevents the flickering of the scene. Apparently some values are lost from the shaders (maybe?) after the rendering pass and they need to be releaded for the next one.
  //this->target_camera->draw(tick,TRANSPARENT,current_program,AND);
}

void GeometryShaderTestRF::perform_rendering(GLint tick, GLuint render_flag, GLuint current_program, Comparator comp,DrawingMode drawing_mode) {
  this->target_camera->select_render_target();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  this->target_camera->draw(tick,SHADOWABLE|SHADOWCASTER,current_program,NOT,ADJACENCY);
}
