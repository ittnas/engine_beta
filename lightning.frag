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

vec3 ads(in uint light_index, in vec4 position, in vec3 normal) {
  vec4 light_position = lights[light_index].position;
  vec3 distance_vector = vec3(light_position - position);
  vec3 n = normalize(normal);
  vec3 s = normalize(distance_vector);
  vec3 v = normalize(vec3(-position));
  vec3 r = reflect(-s,n);
  
  float distance2 = dot(distance_vector,distance_vector);
  float distance = sqrt(distance2);

  vec3 I =  lights[light_index].intensity.xyz;
  //vec3 res_ambient = I*ambient_color.xyz;
  vec3 res_diff_spec = I*(diffusive_color.xyz*max(dot(s,n),0.0) + specular_color.xyz*pow(max(dot(r,v),0.0),shininess));
  res_diff_spec = res_diff_spec/(lights[light_index].kc + lights[light_index].kl*distance + lights[light_index].kq*distance2);
  //return res_ambient + res_diff_spec;
  return res_diff_spec;
}

void main() {
  frag_color = vec4(0,0,0,1);
  
  for(uint i = 0; i < nbr_active_lights; i++) {
    if(lights[i].is_active) {
      frag_color += vec4(ads(i,vertex_data.world_position,vertex_data.world_normal),0.0);
    }
  }
  
}
