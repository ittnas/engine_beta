#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <GL/glew.h>
// THIS IS NOT USED FOR ANYTHING. ALL THE FUNCTIONALITY IS IN THE CAMERE CLASS
class Screen {
private:
  GLuint max_nbr_invocations;
public:
  Screen();
  virtual ~Screen() {}
};

#endif
