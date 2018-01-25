#version 410

layout(triangles) in;
//layout(triangles);
layout(line_strip, max_vertices=3) out;

in VertexData {
  vec4 world_position;
  vec3 world_normal;
  vec2 texture_coordinate;
} vertex_in[3];

out VertexData {
  vec4 world_position;
  vec3 world_normal;
  vec2 texture_coordinate;
} vertex_out;


void main() {

  for(int i = 0; i < 3; i++) {
    gl_Position = gl_in[i].gl_Position;
    //gl_PointSize = 4;
    vertex_out.world_position = vertex_in[i].world_position;
    vertex_out.world_normal = vertex_in[i].world_normal;
    vertex_out.texture_coordinate = vertex_in[i].texture_coordinate;
    EmitVertex();
  }
  EndPrimitive();

  // This draws normals
  /*
  for(int i = 0; i < 3; i++) {
    gl_Position = gl_in[i].gl_Position;
    vertex_out.world_position = vertex_in[i].world_position;
    vertex_out.world_normal = vertex_in[i].world_normal;
    vertex_out.texture_coordinate = vertex_in[i].texture_coordinate;
    EmitVertex();
    gl_Position = gl_in[i].gl_Position + vec4(vertex_in[i].world_normal,0);
    vertex_out.world_position = vertex_in[i].world_position;
    vertex_out.world_normal = vertex_in[i].world_normal;
    vertex_out.texture_coordinate = vertex_in[i].texture_coordinate;
    EmitVertex();
    EndPrimitive();
  }
  */
  // This draws a duplicate
  /*
  for(int i=0;i<3;i++) {
    gl_Position = gl_in[i].gl_Position + vec4(20,0,0,0);
    vertex_out.camera_pos = vertex_in[i].camera_pos;
    vertex_out.camera_normal = vertex_in[i].camera_normal;
    vertex_out.tex_coord = vertex_in[i].tex_coord;
    EmitVertex();
  }
  EndPrimitive();
  */
}
