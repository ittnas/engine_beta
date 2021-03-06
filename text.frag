#version 410

out vec4 frag_color;

uniform sampler2D character_texture;

in VertexData {
  vec4 world_position;
  vec3 world_normal;
  vec2 texture_coordinate;
} vertex_data;

void main() {
  //frag_color = vec4(1,0,texture2D(character_texture,vertex_data.texture_coordinate).a,1);
  frag_color = vec4(0.5,0,0.2,texture2D(character_texture,vertex_data.texture_coordinate).a);
  //frag_color = vec4(1,0,0,1);
}
