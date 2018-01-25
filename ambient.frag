#version 410

out vec4 frag_color;

in VertexData {
  vec4 world_position;
  vec3 world_normal;
} vertex_data;

const int MAX_NBR_LIGHTS = 15;

layout(std140) uniform model_buffer {
  mat4 model_matrix;
  mat3 normal_matrix;
};

struct LightInfo {
       vec4 position;
       vec4 intensity;
       vec3 direction;
       float exponent;
       float cutoff;
       float kc;
       float kl;
       float kq;
       bool is_active;
};

layout(std140) uniform light_buffer {
  LightInfo lights[MAX_NBR_LIGHTS];
};

layout(std140) uniform nbr_lights_buffer {
  uint nbr_active_lights;
};

layout(std140) uniform uniform_material_buffer {
  vec4 ambient_color;
  vec4 diffusive_color;
  vec4 specular_color;
  float shininess;
};

void main() {
  frag_color = ambient_color;
}
