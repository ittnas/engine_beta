#ifndef FPS_LOGGER_HPP
#define FPS_LOGGER_HPP

#include <mutex>
#include <GL/glew.h>

class FPSLogger {
private:
  GLfloat fps;
  GLfloat * fps_circular_buffer;
  GLint N;
  GLint current;
  GLint Nfilled;
  //std::mutex writing_lock;
  //std::mutex reading_lock;
  std::mutex m;
  GLfloat moving_averaged_fps() const;
public:
  GLfloat get_fps();
  void add_fps(GLfloat fps_reading);
  FPSLogger(GLint N);
  ~FPSLogger();
};

#endif
