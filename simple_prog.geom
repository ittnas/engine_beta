#version 410

//layout(triangles) in;
layout(triangles_adjacency) in;
//layout(line_strip, max_vertices=9) out;
layout(triangle_strip, max_vertices=9) out;

in VertexData {
  vec4 world_position;
  vec3 world_normal;
} vertex_in[];

out VertexData {
  vec4 world_position;
  vec3 world_normal;
} vertex_out;

out float vertex_number;


void main() {

    for(int i = 0; i < 6; i++) {
    gl_Position = gl_in[i].gl_Position;
    //gl_PointSize = 4;
    vertex_out.world_position = vertex_in[i].world_position;
    vertex_out.world_normal = vertex_in[i].world_normal;
    vertex_number = i;
    EmitVertex();
  }
  EndPrimitive();
  
  // gl_Position = vec4(-0.1,0,-0.5,1);
  // //gl_PointSize = 4;
  // vertex_out.world_position = vertex_in[0].world_position;
  // vertex_out.world_normal = vertex_in[0].world_normal;
  // vertex_out.texture_coordinate = vertex_in[0].texture_coordinate;
  // vertex_number = 0;
  // EmitVertex();
  // gl_Position = vec4(+0.1,0.0,-0.5,1);
  // vertex_out.world_position = vertex_in[1].world_position;
  // vertex_out.world_normal = vertex_in[1].world_normal;
  // vertex_out.texture_coordinate = vertex_in[1].texture_coordinate;
  // vertex_number = 1;
  // EmitVertex();
  // gl_Position = vec4(0.10,0.1,-0.5,1);
  // vertex_out.world_position = vertex_in[2].world_position;
  // vertex_out.world_normal = vertex_in[2].world_normal;
  // vertex_out.texture_coordinate = vertex_in[2].texture_coordinate;
  // vertex_number = 2;
  // EmitVertex();
  // EndPrimitive();
}
