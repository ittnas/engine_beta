#include <iostream>
#include "fps_logger.hpp"

/**
   A threadsafe fps logger, that uses moving average to calculate the fps.
 **/

FPSLogger::FPSLogger(GLint N = 1) : N(N),current(0),Nfilled(0) {
  if(N<1) this->N = 1;
  fps_circular_buffer = new GLfloat[this->N];
}

FPSLogger::~FPSLogger() {
  delete[] fps_circular_buffer;
}

void FPSLogger::add_fps(GLfloat fps_reading) {
  //std::cout << "Writer acquiring lock." << std::endl;
  std::lock_guard<std::mutex> lk(m);
  //std::cout << "Lock acquired by writer." << std::endl;
  current = (current + 1) % N;
  //std::cout << "Current:" << current << std::endl;
  if(Nfilled < N)
    Nfilled++;
  fps_circular_buffer[current] = fps_reading;
  //std::cout << "Lock freed by writer." << std::endl;
}

GLfloat FPSLogger::get_fps() {
  //std::cout << "Reader acquiring lock." << std::endl;
  std::lock_guard<std::mutex> lk(m);
  //std::cout << "Lock acquired by reader." << std::endl;
  if(Nfilled == 0) return 0.0; // Not actually required (moving_averaged_fps takes care of this) but its here for safety.
  return moving_averaged_fps();
  //std::cout << "Lock freed by reader." << std::endl;
}

/** Calculates the moving average of the circular buffer. This function is not threadsafe, and therefore the methods feeding the buffer must take care of the locking.
 **/

GLfloat FPSLogger::moving_averaged_fps() const {
  GLfloat avg = 0.0;
  //std::cout << "Circular buffer:" << std::endl;
  for(GLint ii=0; ii<Nfilled;ii++) {
    GLint modulo = (current - ii) % N; // Is not equal to the mathemical modulo for negative arguments.
    if(modulo < 0)
      modulo = modulo + N;
    avg += 1.0/fps_circular_buffer[modulo];
    //std::cout << fps_circular_buffer[modulo] << ", ";
  }
  //std::cout << std::endl << "Duration of " << Nfilled << " Frames: " << avg << ". That means fps=" << 1.0/avg*Nfilled << std::endl;
  return 1.0/avg*Nfilled;
}
