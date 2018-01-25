#version 410

out vec4 frag_color;

in vec4 world_position;

const int MAX_NBR_LIGHTS = 15;

layout(std140) uniform model_buffer {
  mat4 model_matrix;
  mat3 normal_matrix;
};

layout(std140) uniform projection_buffer {
  mat4 projection_matrix;
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

void main() {
  //frag_color = vec4(lights[0].position.x,0,0,1);
  //frag_color = vec4(1.0,0,0,0);

  frag_color = vec4(0.0,0,0,0);
  
  /*
  for(uint i = 0; i < nbr_active_lights; i++) {
    frag_color += lights[i].intensity*(sqrt(dot(lights[i].position.xyz - world_position.xyz,lights[i].position.xyz - world_position.xyz)))/10;
    }
  frag_color.a = 1.0;
  */
  frag_color = vec4(0.0,1.0,0.0,1.0);
}
