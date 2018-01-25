#version 410

out vec4 frag_color;

in float vertex_number;

void main() {
  frag_color = vec4(vertex_number/3,0,0,1);
}
