#ifndef RENDERING_FUNCTION_HPP
#define RENDERING_FUNCTION_HPP

#include <GL/glew.h>

#include "render_flag.hpp"
#include "mesh.hpp"


class World;
class Camera;

class RenderingFunction {
protected:
  World * target_world;
  Camera * target_camera;
public:
  virtual void perform_rendering(GLint tick, GLuint render_flag = -1, GLuint current_program = 0, Comparator comp = ANY,DrawingMode drawing_mode = NOTSET) = 0;
  void set_target_camera(Camera * camera);
  RenderingFunction() : target_world(0), target_camera(0) {}
  virtual ~RenderingFunction() {}
};

class OITRenderingFunction : public RenderingFunction {
public:
  OITRenderingFunction() : RenderingFunction() {}
  virtual void perform_rendering(GLint tick, GLuint render_flag = -1, GLuint current_program = 0, Comparator comp = ANY,DrawingMode drawing_mode = NOTSET);
  virtual ~OITRenderingFunction() {}
};

class ShadowVolumesRenderingFunction : public RenderingFunction {
public:
  ShadowVolumesRenderingFunction() : RenderingFunction() {}
  virtual void perform_rendering(GLint tick, GLuint render_flag = -1,GLuint current_program = 0, Comparator comp = ANY,DrawingMode drawing_mode = NOTSET);
  virtual ~ShadowVolumesRenderingFunction() {}
};

class GeometryShaderTestRF : public RenderingFunction {
public:
  GeometryShaderTestRF() : RenderingFunction() {}
  virtual void perform_rendering(GLint tick, GLuint render_flag = -1,GLuint current_program = 0, Comparator comp = ANY,DrawingMode drawing_mode = NOTSET);
  virtual ~GeometryShaderTestRF() {}
};

#endif
