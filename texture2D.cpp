#include <iostream>
#include <bitset>
#include "texture2D.hpp"
#include "opengl_data_type_enum_length.hpp"

Texture2D::Texture2D(GLubyte * const data, GLint internal_format, GLenum format, GLenum type, GLsizei width, GLsizei height) : Texture(internal_format,format,type, GL_TEXTURE_2D), width(width), height(height) {
  this->data = copy_data(data);
  // correct texture should already be bound in the parent constructor.
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(target,0, internal_format,width,height,0,format,type,data);
  //glPixelStorei(GL_PACK_ALIGNMENT, 4);
  //TODO move these to the object (and change the target etc.)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		  GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		  GL_NEAREST);
  /* DEBUGGING
  GLint tb;
  glGetIntegerv(GL_TEXTURE_BINDING_2D,&tb);
  glGetIntegerv(GL_ACTIVE_TEXTURE,&tb);
  GLint active_tex = tb - GL_TEXTURE0;
  glBindTexture(target,0);
  tb;
  glGetIntegerv(GL_TEXTURE_BINDING_2D,&tb);
  glGetIntegerv(GL_ACTIVE_TEXTURE,&tb);
  active_tex = tb - GL_TEXTURE0;
  */
}

GLsizei Texture2D::get_data_length_in_bytes() {
  return width*height*PixelType::nbr_elements_per_pixel(format)*DataType::sizeof_enum(type);
}

 void Texture2D::print_texture() {
   std::cout << "Printing texture:" << std::endl;
   //std::cout << PixelType::nbr_elements_per_pixel(format) << ", " << DataType::sizeof_enum(type) << std::endl;
   for(GLint ii=0;ii<height;ii++) {
     for(GLint ll=0;ll<width;ll++) {
       for(GLint jj=0;jj<PixelType::nbr_elements_per_pixel(format);jj++) {
	 for(GLint kk=0;kk<DataType::sizeof_enum(type);kk++) {
	   //std::bitset<8> x(data[ll*DataType::sizeof_enum(type)*PixelType::nbr_elements_per_pixel(format)*height + ii*DataType::sizeof_enum(type)*PixelType::nbr_elements_per_pixel(format) + jj*DataType::sizeof_enum(type) + kk]);
	   //std::cout << x;
	   std::cout << std::to_string(data[ii*DataType::sizeof_enum(type)*PixelType::nbr_elements_per_pixel(format)*width + ll*DataType::sizeof_enum(type)*PixelType::nbr_elements_per_pixel(format) + jj*DataType::sizeof_enum(type) + kk]);
	 }
	 std::cout << "\t";
	 //std::cout << ", ";
       }
       //std::cout << "; ";
     }
     std::cout << std::endl;
   }
 }

