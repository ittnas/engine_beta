#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

// Its not world position in reality, but position in view space because view matrix is the first element in the modelstack
out VertexData {
  vec4 world_position;
  vec3 world_normal;
} vertex_out;

layout(std140) uniform model_buffer {
  mat4 model_matrix;
  mat3 normal_matrix;
};

layout(std140) uniform view_buffer {
  mat4 view_matrix;
};

layout(std140) uniform projection_buffer {
  mat4 projection_matrix;
  float tick;
};

void main()
{
  //camera_pos =  model_to_camera_matrix * position;
  //camera_normal =  normal_trans_matrix*normal;
  //mat3 temp = transpose(inverse(mat3(model_to_camera_matrix)));
  //camera_normal = temp*normal;
  //camera_normal = abs(vec3(normal_trans_matrix[1]));
  //camera_normal = normal; //THIS IS FOR DEBUGGING!!!
 //camera_normal = normal_trans_matrix*normal;
  vertex_out.world_position = model_matrix*position;
  vertex_out.world_normal = normal_matrix*normal;
  //gl_Position = camera_to_clip_matrix * model_to_camera_matrix * position;
  //tex_coord_out = tex_coord;
  //gl_Position = projection_matrix*view_matrix*model_matrix*position;
  //gl_Position = projection_matrix*vertex_out.world_position;
  gl_Position = vertex_out.world_position;
  gl_PointSize = 5;
  //gl_Position = position;
}
