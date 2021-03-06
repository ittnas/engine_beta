#include "parametric_functions.hpp"

glm::vec3 ParametricFunctions::parametric_sphere(GLfloat u,GLfloat r) {
  GLfloat x = 10*sin(r)*cos(u);
  GLfloat y = 10*sin(r)*sin(u);
  GLfloat z = 10*cos(r);
  return glm::vec3(x,y,z);
}

glm::vec3 ParametricFunctions::parametric_plane(GLfloat u,GLfloat r) {
GLfloat x = 0;
GLfloat y = u;
GLfloat z = r;
return glm::vec3(x,y,z);
}

glm::vec3 ParametricFunctions::parametric_torus(GLfloat u,GLfloat r) {
  GLfloat c = 10;
  GLfloat a = 3;
  GLfloat x = (c + a*cos(u))*cos(r);
  GLfloat y = (c + a*cos(u))*sin(r);
  GLfloat z = a*sin(u);
  return glm::vec3(x,y,z);
}

glm::vec3 ParametricFunctions::parametric_shell(GLfloat u,GLfloat r) {
  GLfloat a = 10.0;
  GLfloat x=a*(2*(1-exp(u/(6*M_PI)))*cos(u)*cos(r/2)*cos(r/2));
  GLfloat y=a*(2*(-1+exp(u/(6*M_PI)))*sin(u)*cos(r/2)*cos(r/2));
  GLfloat z=a*(1-exp(u/(3*M_PI))-sin(r)+exp(u/(6*M_PI))*sin(r));

  return glm::vec3(x,y,z);
}
