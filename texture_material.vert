#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coordinate;

out VertexData {
  vec4 world_position;
  vec3 world_normal;
  vec2 texture_coordinate;
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
  gl_Position = projection_matrix*model_matrix*position;
  vertex_out.texture_coordinate = texture_coordinate;
  //gl_Position = position;
}
