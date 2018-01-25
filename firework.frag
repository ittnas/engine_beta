#version 410

out vec4 frag_color;
in vec3 color_out;

layout(std140) uniform projection_buffer {
  mat4 projection_matrix;
  float t;
};

layout(std140) uniform firework_buffer {
  vec3 initial_velocity;
  float creation_time;
  float explosion_time;
};

void main() {
  vec3 dimm_color = color_out*(1-(t-creation_time)/4);
  frag_color = vec4(dimm_color,1);
  //frag_color = vec4(initial_velocity,1);
}
