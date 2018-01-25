#ifndef KEY_TABLE_HPP
#define KEY_TABLE_HPP

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <unordered_map>
#include <vector>

class KeyTable {
private:
  static const GLuint nbr_keys = 60;
  const static GLuint symbols[];
  static std::unordered_map<GLuint,GLubyte> keys; //Key, status
  static GLboolean initialized;
public:
  KeyTable();
  ~KeyTable();
  std::vector<GLuint> get_active_keys();
  void set_status(GLuint key,GLubyte status);
  GLubyte get_status(GLuint key);
  void toggle(GLuint key);
};
  
#endif
  
