#include <iostream>

#include "program.hpp"
#include "buffer_list.hpp"
#include "uniform_list.hpp"


void Program::set_uniform_block_binding(std::string ubo_name, GLuint binding_point) {
  GLuint ubo_index = glGetUniformBlockIndex(get_program_ident(),ubo_name.c_str());
  //assert(ubo_index != GL_INVALID_INDEX); 
  //Last parameter should be global_ubo_binding_point
  glUniformBlockBinding(get_program_ident(),ubo_index,binding_point);
  
}

Program::Program(const std::string &vertex_shader_file_name, const std::string &fragment_shader_file_name,const std::string &geometry_shader_file_name) {
  GLuint vertex_shader_ident = load_shader(GL_VERTEX_SHADER,vertex_shader_file_name);
  GLuint fragment_shader_ident = load_shader(GL_FRAGMENT_SHADER,fragment_shader_file_name);
  GLuint shader_program_ident = glCreateProgramObjectARB();
  glAttachObjectARB(shader_program_ident,vertex_shader_ident);
  glAttachObjectARB(shader_program_ident,fragment_shader_ident);

  if(!geometry_shader_file_name.empty()) {
    GLuint geometry_shader_ident = load_shader(GL_GEOMETRY_SHADER,geometry_shader_file_name);
    glAttachObjectARB(shader_program_ident,geometry_shader_ident);
  }

  glLinkProgramARB(shader_program_ident);
  //glUseProgramObjectARB(shader_program_ident);
  //glUseProgram(shader_program_ident);
  this->program_ident = shader_program_ident;
  // Next the block binding.
  // First query for uniform block names.
  GLint max_name_buf_size;
  glGetProgramiv(shader_program_ident,GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH,&max_name_buf_size);
  GLchar * ubo_name_buf = (GLchar *)malloc(sizeof(GLchar)*max_name_buf_size);
  if(ubo_name_buf == NULL) {
    assert(0);
  }
  GLsizei nbr_read_characters;
  GLint nbr_active_uniform_blocks;
  glGetProgramiv(shader_program_ident,GL_ACTIVE_UNIFORM_BLOCKS,&nbr_active_uniform_blocks);
  for(GLint i = 0; i < nbr_active_uniform_blocks; i++) {
    glGetActiveUniformBlockName(shader_program_ident,i,max_name_buf_size,&nbr_read_characters,ubo_name_buf);
    /*
    for(GLint j = 0; j < nbr_read_characters; j++) {
      std::cout << ubo_name_buf[j];
    }
    std::cout << std::endl;
    */
    std::string name_string(ubo_name_buf,nbr_read_characters);
    std::cout << name_string << std::endl;
    set_uniform_block_binding(name_string,BufferList::get_binding_point(name_string));
  }
  free(ubo_name_buf);
  // Add uniform locations to UniformList
  GLint max_uniform_name_size;
  glGetProgramiv(shader_program_ident,GL_ACTIVE_UNIFORM_MAX_LENGTH,&max_uniform_name_size);
  GLchar * uniform_name_buf = (GLchar *)malloc(sizeof(GLchar)*max_uniform_name_size);
  if(ubo_name_buf == NULL) {
    assert(0);
  }
  GLint nbr_active_uniforms;
  glGetProgramiv(shader_program_ident,GL_ACTIVE_UNIFORMS,&nbr_active_uniforms);
  for(GLuint i = 0; i < nbr_active_uniforms; i++) {
    GLenum uniform_type;
    GLint size;
    glGetActiveUniform(shader_program_ident,i,max_uniform_name_size,&nbr_read_characters,&size,&uniform_type,uniform_name_buf);
    GLint uniform_location = glGetUniformLocation(shader_program_ident,uniform_name_buf);
    if(uniform_location != -1) {
    std::string name_string(uniform_name_buf,nbr_read_characters);
    std::cout << name_string << ": " << uniform_location << std::endl;
    UniformList::add_uniform(name_string,shader_program_ident,uniform_location);
    }
  }
  free(uniform_name_buf);
}

GLuint Program::load_shader(GLenum shader_type, const std::string &shader_file_name) {
  // Look for local name first
  std::string full_name = LOCAL_FILE_DIR + shader_file_name;
  std::ifstream test_file(full_name.c_str());
  GLuint shader_ident;
  if(test_file.is_open()) {
    shader_ident = create_shader(shader_type,test_file);
    return shader_ident;
  }
  full_name = GLOBAL_FILE_DIR + shader_file_name;
  std::ifstream test_file2(full_name.c_str());
    if(test_file2.is_open()) {
      shader_ident = create_shader(shader_type,test_file2);
      return shader_ident;
    }
    throw std::runtime_error("Could not find the file " + shader_file_name);
}

GLuint Program::create_shader(GLenum shader_type, std::ifstream &shader_file_stream) {
  std::stringstream shader_data;
  shader_data << shader_file_stream.rdbuf();
  //std::cout << shader_data.str() << std::endl;
  shader_file_stream.close();
  GLuint shader_ident = glCreateShader(shader_type);
  std::string temp_string = shader_data.str();
  const GLchar * source_string =temp_string.c_str(); // If this is done in one step the data the pointer points to gets destroyd after this line. The shader will then be randomly corrupted.
  //const GLchar * source_string = shader_data.str().c_str();
  glShaderSource(shader_ident,1,&source_string,NULL);
  glCompileShaderARB(shader_ident);
  GLint status = 0;

  glGetShaderiv(shader_ident,GL_COMPILE_STATUS,&status);
  if(!status) {
    //if(1) { //Always print infolog
    GLsizei maxlength = 200;
    GLsizei * length = (GLsizei *)malloc(sizeof(GLsizei));
    GLchar * infolog = (GLchar *)malloc(sizeof(GLchar)*maxlength);
    glGetShaderInfoLog(shader_ident,maxlength,length,infolog);
    std::cout << "Compilation error:" << std::endl << infolog;
    free(length);
    free(infolog);
  }
  std::cout << shader_type << ": " << status << std::endl;
  
  return shader_ident;
}

void Program::list_uniforms() {
  GLint total = -1;
  glGetProgramiv(get_program_ident(),GL_ACTIVE_UNIFORMS, &total);
  for(GLint i= 0; i < total; i++) {
    int name_len = -1, num = -1;
    GLenum type = GL_ZERO;
    char name[100];
    glGetActiveUniform(get_program_ident(),GLuint(i), sizeof(name)-1,&name_len,&num,&type,name);
    name[name_len] = 0;
    GLuint location = glGetUniformLocation(get_program_ident(),name);
    std::cout << name << ": " << location << std::endl;
  }

}
