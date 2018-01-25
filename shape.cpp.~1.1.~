#include "shape.hpp"

void Shape::draw(DrawingMode drawing_mode) {
  mesh->draw(drawing_mode);
}
Shape::Shape() : mesh(0) {
}

Shape::~Shape() {
  delete mesh;
};

void Shape::add_to_vector(std::vector<GLfloat> & output, glm::vec4 & target) {
  for(GLuint i = 0; i < 4; i++) {
    output.push_back(target[i]);
  }
}

