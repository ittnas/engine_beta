#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GL/glew.h>
#include "matrix_stack.hpp"

/** 
 * Creates a matrix stack with an identity matrix on top.
 * 
 */
MatrixStack::MatrixStack() {
  stack.push(glm::mat4(1.0));
}

/** 
 * Pops the top matrix out of the stack.
 * 
 */
void MatrixStack::pop() {
  stack.pop();
}

/** 
 * Pushes a new matrix on top of the stack, copying the contents of the previous top matrix.
 * 
 */

void MatrixStack::push() {
  stack.push(stack.top());
}

/** 
 * Loads an identity matrix to the top matrix, replacing its content.
 * 
 */

void MatrixStack::load_identity() {
  stack.top() = glm::mat4(1.0);
}

void MatrixStack::load_matrix(glm::mat4 new_mat) {
  stack.top() = new_mat;
}

void MatrixStack::translate(const glm::vec4 & trans) {
  glm::mat4 trans_mat(1.0);
  trans_mat[3] = trans;
  stack.top() = stack.top()*trans_mat;
}

void MatrixStack::translate(const glm::vec3 & trans) {
  glm::mat4 trans_mat(1.0);
  trans_mat[3] = glm::vec4(trans,1.0);
  stack.top() = stack.top()*trans_mat;
}

void MatrixStack::scale(const glm::vec3 & scale) {
  glm::mat4 scale_mat(1.0);
  scale_mat[0][0] = scale.x;
  scale_mat[1][1] = scale.y;
  scale_mat[2][2] = scale.z;
  stack.top() = stack.top()*scale_mat;
}

void MatrixStack::scale(const GLdouble scale) {
  glm::mat4 scale_mat(1.0);
  scale_mat[3][3] = 1/scale;
  stack.top() = stack.top()*scale_mat;
}

glm::mat4 MatrixStack::top() {
  return stack.top();
}

/*
Rotates the model matrix around the axis amount specified by angle in radians.

 */

void MatrixStack::rotate(GLfloat angle,const glm::vec3 & axis) {
  
  //No need for this, glm can already handle this all.
  /*
  GLfloat x = axis.x;
  GLfloat y = axis.y;
  GLfloat z = axis.z;


  GLfloat theta = theta*pi/180; // Angle in radians
  GLfloat C = cos(theta);
  GLfloat S = sin(theta);
  GLfloat iC = 1 - C;
  GLfloat iS = 1 - S;
  
  // Rotation matrix in column major order.
  GLfloat arr[16] = {x*x + (1 - x*x)*C,iC*x*y + z*S,iC*x*z - y*S,0,
		     iC*x*y - z*S, y*y + (1 - y*y)*C,iC*y*z + x*S,0,
		     iC*x*z + y*S,iC*y*z - x*S, z*z + (1 - z*z)*C,0,
		     0,0,0,1};
  */

  //glm::mat4 trans_mat = glm::gtx::transform::rotate(angle,axis);
  glm::mat4 trans_mat = glm::rotate(angle,axis);
  stack.top() = stack.top()*trans_mat;
}

/*
  Rotates the model matrix amount specified a quaternion.
 */

void MatrixStack::rotate(const glm::fquat & orientation) {
  stack.top() = stack.top() * glm::mat4_cast(orientation);
}
/*
void MatrixStack::set(glm::mat4 new_mat){
  stack.top() = new_mat;
}
*/

/** 
 * Multiplies the given input matrix by the top matrix of the matrix stack elementwise. Output is written to output vector.
 * 
 * @param input Vector to be multiplied.
 * @param output Vector into which the output is written. If vector is not empty, the output is written after the elements.
 * 
 * @return 
 */

void MatrixStack::mult_array(const std::vector<glm::vec4> & input, std::vector<glm::vec4> & output) {
  output.reserve(input.size() + output.size());
  for(GLuint i = 0; i < input.size(); i++) {
    output.push_back(top()*input[i]);
  }
}

/** 
 *  Multiplies the given input matrix by the top matrix of the matrix stack elementwise. Output is written to output vector. Each vec3 element is appended with 1 inorder to transform them to vec4 elements.
 * 
 * @param input Vector to be multiplied by the matrix stack.
 * @param output Vector into which the output is written. If vector is not empty, the output is written after the elements.
 * 
 * @return 
 */


void MatrixStack::mult_array(const std::vector<glm::vec3> & input, std::vector<glm::vec3> & output) {
 output.reserve(input.size() + output.size());
  for(GLuint i = 0; i < input.size(); i++) {
    glm::vec4 temp = top()*glm::vec4(input[i].x,input[i].y,input[i].z,1);
    output.push_back(glm::vec3(temp.x,temp.y,temp.z));
  }
}

 /** 
   * Multiplies the given input matrix by the top matrix of the matrix stack elementwise. Output is written to output vector. Size determines the number of points in one element. Default value is 4. If 3 is given, 1 is appended after the element.
   * 
   * @param input Input vector.
   * @param output Output vector.
   * @param size Amount of points in one element of the vector.
   * 
   * @return 
   */


void MatrixStack::mult_array(const std::vector<GLfloat> & input, std::vector<GLfloat> output, GLuint size) {
  if(size < 3 || size > 4) {
    return;
  }
  GLuint length = input.size();
  if(size == 4) {
    GLuint nbr_elements = length/size;
    for(GLuint i = 0; i < nbr_elements; i++) {
      glm::vec4 temp(input[i*4],input[i*4+1],input[i*4+2],input[i*4+3]);
      glm::vec4 result = top()*temp;
      output.push_back(result.x);
      output.push_back(result.y);
      output.push_back(result.z);
      output.push_back(result.w);
    }
  } else if(size == 3) {
    GLuint nbr_elements = length/size;
    for(GLuint i = 0; i < nbr_elements; i++) {
      glm::vec4 temp(input[i*3],input[i*3+1],input[i*3+2],1);
      glm::vec4 result = top()*temp;
      output.push_back(result.x);
      output.push_back(result.y);
      output.push_back(result.z);
    }
  }
}

void MatrixStack::mult_array(const GLfloat * input, GLfloat * output, GLuint size, GLuint nbr_elements) {
  if(size < 3 || size > 4) {
    return;
  }
  GLuint length = nbr_elements;
  if(size == 4) {
    GLuint nbr_elements = length/size;
    for(GLuint i = 0; i < nbr_elements; i++) {
      glm::vec4 temp(input[i*4],input[i*4+1],input[i*4+2],input[i*4+3]);
      glm::vec4 result = top()*temp;
      output[i*4] = result.x;
      output[i*4+1] = result.y;
      output[i*4+2] = result.z;
      output[i*4+3] = result.w;
    }
  } else if(size == 3) {
    GLuint nbr_elements = length/size;
    for(GLuint i = 0; i < nbr_elements; i++) {
      glm::vec4 temp(input[i*3],input[i*3+1],input[i*3+2],1);
      glm::vec4 result = top()*temp;
      output[i*3] = result.x;
      output[i*3+1] = result.y;
      output[i*3+2] = result.z;
    }
  }
}

void MatrixStack::mult_normal_array(const GLfloat * input, GLfloat * output, GLuint nbr_elements) {
  glm::mat3 normal_trans_matrix(top());
  normal_trans_matrix = glm::transpose(glm::inverse(normal_trans_matrix));
  push();
  glm::mat4 temp = glm::mat4(normal_trans_matrix);
  load_matrix(glm::mat4(normal_trans_matrix));
  mult_array(input,output,3,nbr_elements);
  pop();
}


