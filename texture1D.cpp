#include <iostream>
#include "texture1D.hpp"
#include "opengl_data_type_enum_length.hpp"

Texture1D::Texture1D(GLubyte * const data,GLint internal_format,GLenum format,GLenum type,GLsizei width) : Texture(internal_format,format,type,GL_TEXTURE_1D), width(width) {
  this->data = copy_data(data);//This DATA is now MINE! (you can do with yours whatever you want to).
  
  glTexImage1D(target,0,internal_format, width,0, format, type, data);
  glBindTexture(target,0);
}

GLsizei Texture1D::get_data_length_in_bytes() {
  return width*PixelType::nbr_elements_per_pixel(format)*DataType::sizeof_enum(type);
}

void Texture1D::print_texture() {
  std::cout << "Printing texture:" << std::endl;
  for(GLint ii=0;ii<width;ii++) {
    for(GLint jj=0;jj<PixelType::nbr_elements_per_pixel(format);jj++) {
      for(GLint kk=0;kk<DataType::sizeof_enum(type);kk++) {
	std::cout << data[ii*DataType::sizeof_enum(type)*PixelType::nbr_elements_per_pixel(format) + jj*DataType::sizeof_enum(type) + kk];
      }
      std::cout << ", ";
    }
    std::cout << "; ";
  }
  std::cout << std::endl;
}
