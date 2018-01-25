#include <iostream>

#include "parametric_plot.hpp"
#include "parametric_functions.hpp"

ParametricPlot::ParametricPlot(ParametricSurface function, glm::vec2 ulim,glm::vec2 rlim,GLuint nu, GLuint nr, GLboolean uclosed,GLboolean rclosed) : ParametricPlot(std::unique_ptr<ParametricSurfaceGeneratorFromFunction>(new ParametricSurfaceGeneratorFromFunction(function)),ulim,rlim,nu,nr,uclosed,rclosed) {
  //set_mesh();
}

ParametricPlot::ParametricPlot(std::unique_ptr<ParametricSurfaceGenerator> generator, glm::vec2 ulim,glm::vec2 rlim,GLuint nu, GLuint nr, GLboolean uclosed,GLboolean rclosed) : generator(std::move(generator)), ulim(ulim), rlim(rlim), nu(nu), nr(nr),uclosed(uclosed),rclosed(rclosed) {
  set_mesh();
}

Mesh * ParametricPlot::create_mesh() {
  Mesh * mesh = new Mesh(QUAD,INDEXED);


  GLuint nud = uclosed ? nu : nu+1; //number of u data points
  GLuint nrd = rclosed ? nr : nr+1; //number of r data points
  GLfloat * data = new GLfloat[nud*nrd*4];

  GLfloat du = (ulim.y - ulim.x)/(nu);
  GLfloat dr = (rlim.y - rlim.x)/(nr);

  for(GLuint i = 0; i < nud; i++) {
    for(GLuint j = 0; j <nrd; j++) {
      //glm::vec3 fur = function(ulim.x + i*du,rlim.x+j*dr);
      glm::vec3 fur = generator->get_value_at(ulim.x + i*du,rlim.x+j*dr);
      GLuint index = (i*nrd + j)*4;
      data[index] = fur.x;
      data[index+1] = fur.y;
      data[index+2] = fur.z;
      data[index+3] = 1.0;
    }
  }

  //Calculate the normal. This is not perfect if the shape is continuos, because the normal at the last index won't be correct.
  GLfloat * normal_data = new GLfloat[nud*nrd*3];
  // normal = (dfdu x dfdr)/|dfdu x dfdr|
  for(GLuint i = 1; i < nud-1; i++) {
    for(GLuint j = 1; j < nrd-1; j++) {
      //glm::vec3 normal = glm::cross(glm::vec3(
      // Current index in normal array
      GLuint curr = (i*nrd + j)*3;

	// Indices in data array.
      GLuint up = ((i-1)*nrd + j)*4;
      GLuint un = ((i+1)*nrd + j)*4;
      GLuint rp = (i*nrd + j-1)*4;
      GLuint rn = (i*nrd + j+1)*4;
      // Finite difference
      glm::vec3 dfdu = (glm::vec3(data[un],data[un+1],data[un+2]) - glm::vec3(data[up],data[up+1],data[up+2]))/(2*du);
      glm::vec3 dfdr = (glm::vec3(data[rn],data[rn+1],data[rn+2]) - glm::vec3(data[rp],data[rp+1],data[rp+2]))/(2*dr);
      glm::vec3 normal = glm::cross(dfdu,dfdr);
      // For some reason normals work if this is reversed...
      normal = -glm::normalize(normal);
      
      normal_data[curr] = normal.x;
      normal_data[curr+1] = normal.y;
      normal_data[curr+2] = normal.z;
    }
  }
  //Next fill the edges.
  // Normals appear to break when nu > nr
  
  if(!uclosed) { // If u is open loop over r
    for(GLuint i = 1; i<nrd-1;i++) {
      //GLuint curr = (i*nu + j)*3;
      normal_data[i*3] = normal_data[(nrd + i)*3];
      normal_data[i*3 + 1] = normal_data[(nrd + i)*3 + 1];
      normal_data[i*3 + 2] = normal_data[(nrd + i)*3 + 2];
      normal_data[(nrd*(nud-1)+i)*3] = normal_data[(nrd*(nud-2) + i)*3];
      normal_data[(nrd*(nud-1)+i)*3 + 1] = normal_data[(nrd*(nud-2) + i)*3 + 1];
      normal_data[(nrd*(nud-1)+i)*3 + 2] = normal_data[(nrd*(nud-2) + i)*3 + 2];
    }
  }
  if(!rclosed) { // If r is open loop over u
    for(GLuint i = 1; i<nud-1;i++) {
      //GLuint curr = (i*nu + j)*3;
      normal_data[i*nrd*3] = normal_data[(nrd*i + 1)*3];
      normal_data[i*nrd*3 + 1] = normal_data[(nrd*i + 1)*3 + 1];
      normal_data[i*nrd*3 + 2] = normal_data[(nrd*i + 1)*3 + 2];
      normal_data[(nrd + i*nrd -1)*3] = normal_data[(nrd*i + nrd -2)*3];
      normal_data[(nrd + i*nrd -1)*3 + 1] = normal_data[(nrd*i + nrd -2)*3 + 1];
      normal_data[(nrd + i*nrd -1)*3 + 2] = normal_data[(nrd*i + nrd -2)*3 + 2];
    }
  }
  // Now all the open borders are filled.
  if(uclosed) {
    for(GLuint i = 1; i < nrd-1; i++) {
      GLuint curr = (i)*3;

      // Indices in data array.
      GLuint up = ((nud-1)*nrd + i)*4;
      GLuint un = ((0+1)*nrd + i)*4;
      GLuint rp = (0*nrd + i-1)*4;
      GLuint rn = (0*nrd + i+1)*4;
      // Finite difference
      glm::vec3 dfdu = (glm::vec3(data[un],data[un+1],data[un+2]) - glm::vec3(data[up],data[up+1],data[up+2]))/(2*du);
      glm::vec3 dfdr = (glm::vec3(data[rn],data[rn+1],data[rn+2]) - glm::vec3(data[rp],data[rp+1],data[rp+2]))/(2*dr);
      glm::vec3 normal = glm::cross(dfdu,dfdr);
      // For some reason normals work if this is reversed...
      normal = -glm::normalize(normal);
      
      normal_data[curr] = normal.x;
      normal_data[curr+1] = normal.y;
      normal_data[curr+2] = normal.z;

      curr = (nrd*(nud-1)+i)*3;

      // Indices in data array.
      up = ((nud-2)*nrd + i)*4;
      un = (i)*4;
      rp = ((nud-2)*nrd + i-1)*4;
      rn = ((nud-2)*nrd + i+1)*4;
      // Finite difference
      dfdu = (glm::vec3(data[un],data[un+1],data[un+2]) - glm::vec3(data[up],data[up+1],data[up+2]))/(2*du);
      dfdr = (glm::vec3(data[rn],data[rn+1],data[rn+2]) - glm::vec3(data[rp],data[rp+1],data[rp+2]))/(2*dr);
      normal = glm::cross(dfdu,dfdr);
      // For some reason normals work if this is reversed...
      normal = -glm::normalize(normal);
      
      normal_data[curr] = normal.x;
      normal_data[curr+1] = normal.y;
      normal_data[curr+2] = normal.z;
    }
  }
  if(rclosed) {
    for(GLuint i = 1; i < nud-1; i++) {
      GLuint curr = i*nrd*3;

      // Indices in data array.
      GLuint up = ((i-1)*nrd + 0)*4;
      GLuint un = ((i+1)*nrd + 0)*4;
      GLuint rp = (i*nrd + nrd-1)*4;
      GLuint rn = (i*nrd + 0+1)*4;
      // Finite difference
      glm::vec3 dfdu = (glm::vec3(data[un],data[un+1],data[un+2]) - glm::vec3(data[up],data[up+1],data[up+2]))/(2*du);
      glm::vec3 dfdr = (glm::vec3(data[rn],data[rn+1],data[rn+2]) - glm::vec3(data[rp],data[rp+1],data[rp+2]))/(2*dr);
      glm::vec3 normal = glm::cross(dfdu,dfdr);
      // For some reason normals work if this is reversed...
      normal = -glm::normalize(normal);
      
      normal_data[curr] = normal.x;
      normal_data[curr+1] = normal.y;
      normal_data[curr+2] = normal.z;

      curr = (nrd + i*nrd -1)*3;

      // Indices in data array.
      up = ((i-1)*nrd + nrd-1 +0)*4;
      un = ((i+1)*nrd + nrd-1 +0)*4;
      rp = ((i)*nrd + nrd-1 -1)*4;
      rn = ((i)*nrd)*4;
      // Finite difference
      dfdu = (glm::vec3(data[un],data[un+1],data[un+2]) - glm::vec3(data[up],data[up+1],data[up+2]))/(2*du);
      dfdr = (glm::vec3(data[rn],data[rn+1],data[rn+2]) - glm::vec3(data[rp],data[rp+1],data[rp+2]))/(2*dr);
      normal = glm::cross(dfdu,dfdr);
      // For some reason normals work if this is reversed...
      normal = -glm::normalize(normal);
      
      normal_data[curr] = normal.x;
      normal_data[curr+1] = normal.y;
      normal_data[curr+2] = normal.z;
    }
  }
  
  //Corners
  // Now they only expect that nr is closed. Add average of both.  

  //LL
  normal_data[0] = normal_data[(nrd + nrd - nr)*3];
  normal_data[1] = normal_data[(nrd + nrd - nr)*3 +1];
  normal_data[2] = normal_data[(nrd + nrd - nr)*3 +2];
  //LR
  normal_data[(nrd-1)*3] = normal_data[((nrd-1) + nr)*3];
  normal_data[(nrd-1)*3+1] = normal_data[((nrd-1) + nr)*3+1];
  normal_data[(nrd-1)*3+2] = normal_data[((nrd-1) + nr)*3+2];

  //UL

  normal_data[(nud-1)*nrd*3] = normal_data[((nud-2)*nrd + nrd -nr)*3];
  normal_data[(nud-1)*nrd*3+1] = normal_data[((nud-2)*nrd + nrd -nr)*3+1];
  normal_data[(nud-1)*nrd*3+2] = normal_data[((nud-2)*nrd + nrd -nr)*3+2];

  //UR

   normal_data[(nud*nrd - 1)*3] = normal_data[((nud-2)*nrd -1 + nr)*3];
   normal_data[(nud*nrd - 1)*3+1] = normal_data[((nud-2)*nrd -1 + nr)*3+1];
   normal_data[(nud*nrd - 1)*3+2] = normal_data[((nud-2)*nrd -1 + nr)*3+2];
  
  GLfloat * tex_coord = new GLfloat[nud*nrd*2];
  GLfloat dtexu = (GLfloat)1/nu;
  GLfloat dtexr = (GLfloat)1/nr;
  for(GLuint i = 0; i < nud; i++) {
    for(GLuint j = 0; j < nrd; j++) {
      GLuint cindex = (i*nrd + j)*2;
      tex_coord[cindex] = i*dtexu*1;
      tex_coord[cindex+1] = j*dtexr*1;
    }
  }
  //mesh->add_attribute_array(FLOAT,4,nr*nu,(GLvoid*)data);
  //mesh->add_attribute_array(FLOAT,3,nr*nu,(GLvoid*)normal_data);
  //mesh->add_attribute_array(FLOAT,2,nr*nu,(GLvoid*)tex_coord);

  mesh->add_attribute_array(4,nrd*nud,data);
  mesh->add_attribute_array(3,nrd*nud,normal_data);
  mesh->add_attribute_array(2,nrd*nud,tex_coord);

  
  GLuint * index_array = new GLuint[(nu)*(nr)*4];

  // Something is wrong when nu > nr

  for(GLuint i = 0; i < nu;i++) {
    for(GLuint j = 0; j < nr;j++) {
      GLuint bi = ((i+1) % (nud)) ? i+1 : 0;
      GLuint bj = ((j+1) % (nrd)) ? j+1 : 0;
      GLuint iaindex = (nr*i + j)*4;
      //GLuint daindex = (nrd*i + j);
      GLuint d1 = (nrd*i + j);
      GLuint d2 = (nrd*i + bj);
      GLuint d3 = (nrd*bi+ bj);
      GLuint d4 = (nrd*bi+ j);

      //      index_array[iaindex] = daindex;
      //index_array[iaindex+1] = daindex + 1;
      //index_array[iaindex+2] = daindex + 1 + nrd;
      //index_array[iaindex+3] = daindex + nrd;

      index_array[iaindex] = d1;
      index_array[iaindex+1] = d2;
      index_array[iaindex+2] = d3;
      index_array[iaindex+3] = d4;

    }
  }
  if(uclosed) {
    
  }
  
  //mesh->set_index_array(UNSIGNED_INT,(nu-1)*(nr-1)*4,(GLvoid *)index_array);
  mesh->set_index_array((nu)*(nr)*4,index_array);
  mesh->convert_to_triangles();
  mesh->create_adjacency_data();
  mesh->save_to_memory();
  mesh->save_index_data_to_memory();
  mesh->save_adjacency_data_to_memory(GL_FALSE);
  delete[] data;
  delete[] normal_data;
  delete[] tex_coord;
  delete[] index_array;
  return mesh;
}

ParametricSphere::ParametricSphere(glm::vec3 center,GLfloat radius,GLuint n) : ParametricPlot(std::unique_ptr<ParametricSphereGenerator>(new ParametricSphereGenerator(center,radius)),glm::vec2(0,2*M_PI),glm::vec2(0,M_PI),n,n,GL_TRUE,GL_FALSE) {
  
}

