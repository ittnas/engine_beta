#ifndef PARAMETRIC_FUNCTIONS_HPP
#define PARAMETRIC_FUNCTIONS_HPP

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace ParametricFunctions {

glm::vec3 parametric_sphere(GLfloat u,GLfloat r);
glm::vec3 parametric_plane(GLfloat u,GLfloat r);
glm::vec3 parametric_torus(GLfloat u,GLfloat r);
glm::vec3 parametric_shell(GLfloat u,GLfloat r);
};

#endif
