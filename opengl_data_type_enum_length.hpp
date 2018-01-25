#ifndef OPENGL_DATA_TYPE_ENUM_LENGTH
#define OPENGL_DATA_TYPE_ENUM_LENGTH

#include <GL/glew.h>
#include <unordered_map>

class DataType {
private:
  static std::unordered_map<GLenum,GLsizei> data_types;
public:
  static GLsizei sizeof_enum(GLenum);
};

class PixelType {
private:
  static std::unordered_map<GLenum,GLsizei> pixel_types;
public:
  static GLsizei nbr_elements_per_pixel(GLenum);
};

#endif
