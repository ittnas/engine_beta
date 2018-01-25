#ifndef DATA_TYPE_HPP
#define DATA_TYPE_HPP

#include <GL/glew.h>

enum Type {
  FLOAT=GL_FLOAT,DOUBLE = GL_DOUBLE,INT = GL_INT,UNSIGNED_INT = GL_UNSIGNED_INT
  };

class DataType {

private:
  Type type;
public:
  

  DataType(Type type) : type(type) {}
  GLuint get_size();
  Type get_type() {return type;}
};


#endif
