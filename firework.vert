#version 410

layout(location = 0) in vec3 speed;
layout(location = 1) in float pointsize;
layout(location = 2) in vec3 color;

out vec3 color_out;

layout(std140) uniform projection_buffer {
  mat4 projection_matrix;
  float t;
};

layout(std140) uniform model_buffer {
  mat4 model_matrix;
  mat3 normal_matrix;
};

layout(std140) uniform firework_buffer {
  vec3 initial_velocity;
  float creation_time;
  float explosion_time;
};

const float g = 0.5;

void main() {
  //gl_Position = vec4(0,-0.5*g*t*t,0,0) + vec4(speed*t,0) + model_matrix*vec4(0,0,0,1);
  color_out = color;
  float dt1 = t - creation_time;
  vec4 expl_pos = (vec4(0,0,0,1)+vec4(initial_velocity*dt1,0) + vec4(0,-0.5*g*dt1*dt1,0,0));

  if(t < explosion_time) {
    gl_PointSize = 10;
    gl_Position = projection_matrix*model_matrix*expl_pos;
  } else {
    gl_PointSize = pointsize;
    float dt = t - explosion_time;
    gl_Position = projection_matrix*model_matrix*((vec4(speed*dt,0) + vec4(0,-0.5*g*dt*dt,0,0)) + expl_pos);
  }
}
