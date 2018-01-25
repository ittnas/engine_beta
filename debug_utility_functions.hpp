#ifndef DEBUG_UTILITY_FUNCTIONS_HPP
#define DEBUG_UTILITY_FUNCTIONS_HPP

#include <glm/glm.hpp>
#include <string>

namespace DebugUtilityFunctions {
  void print(glm::vec3 vec);
  void print(glm::vec4 vec);
  void print(glm::mat3 mat);
  void print(glm::mat4 mat);
  void print(std::string text);
}
#endif
