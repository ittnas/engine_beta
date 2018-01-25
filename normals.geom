#version 400

layout(triangles) in;
layout(line_strip, max_vertices=6) out;

in VertexData {
  vec4 world_position;
  vec3 world_normal;
} vertex_in[3];

out VertexData {
  vec4 world_position;
  vec3 world_normal;
} vertex_out;

layout(std140) uniform view_buffer {
  mat4 view_matrix;
};

layout(std140) uniform projection_buffer {
  mat4 projection_matrix;
  float tick;
};

void main() {
  for(uint i = 0; i < 3; i++) {
    gl_Position = gl_in[i].gl_Position;
    vertex_out.world_position = vertex_in[i].world_position;
    vertex_out.world_normal = vertex_in[i].world_normal;
    EmitVertex();
  }
  EndPrimitive();
  
  // This draws normals
  for(int i = 0; i < 3; i++) {
    gl_Position = gl_in[i].gl_Position;
    vertex_out.world_position = vertex_in[i].world_position;
    vertex_out.world_normal = vertex_in[i].world_normal;
    EmitVertex();
    //gl_Position = gl_in[i].gl_Position + view_matrix*vec4(vertex_in[i].world_normal,0);
    gl_Position = gl_in[i].gl_Position + 5*(1 + sin(tick*6.3*1))*vec4(vertex_in[i].world_normal,0);
    vertex_out.world_position = vertex_in[i].world_position;
    vertex_out.world_normal = vertex_in[i].world_normal;
    EmitVertex();
    EndPrimitive();
  }
}
