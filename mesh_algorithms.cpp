#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <glm/gtx/norm.hpp>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <queue>

#include "mesh_algorithms.hpp"
#include "debug_utility_functions.hpp"

MeshAlgorithms::Sphere * MeshAlgorithms::create_circumsphere(const Tetrahedron & tetr) {
  glm::mat4 a = tetr.get_vertices();
  glm::mat4 atr = glm::transpose(a);
  glm::vec4 c2 = glm::vec4(0,0,0,0);
  for(GLuint ii=0; ii<4;ii++) {
    for(GLuint jj=0;jj<3;jj++) {
      c2[ii] = c2[ii] + a[ii][jj]*a[ii][jj];
    }
  }
  glm::mat4 dx = glm::transpose(glm::mat4(c2,glm::vec4(atr[1]),glm::vec4(atr[2]),glm::vec4(1,1,1,1)));
  glm::mat4 dy = glm::transpose(glm::mat4(c2,glm::vec4(atr[0]),glm::vec4(atr[2]),glm::vec4(1,1,1,1)));
  glm::mat4 dz = glm::transpose(glm::mat4(c2,glm::vec4(atr[0]),glm::vec4(atr[1]),glm::vec4(1,1,1,1)));
  glm::mat4 c = glm::transpose(glm::mat4(c2,glm::vec4(atr[0]),glm::vec4(atr[1]),glm::vec4(atr[2])));
  std::cout << "a" << std::endl;
  DebugUtilityFunctions::print(a);
  std::cout << "dx" << std::endl;
  DebugUtilityFunctions::print(dx);
  std::cout << "dy" << std::endl;
  DebugUtilityFunctions::print(dy);
  std::cout << "dz" << std::endl;
  DebugUtilityFunctions::print(dz);
  std::cout << "c" << std::endl;
  DebugUtilityFunctions::print(c);

  GLfloat deta = glm::determinant(a);
  GLfloat detx = glm::determinant(dx);
  GLfloat dety = -glm::determinant(dy);
  GLfloat detz = glm::determinant(dz);
  GLfloat detc = glm::determinant(c);
  std::cout << "|a|" << std::endl;
    std::cout << deta << std::endl;
  std::cout << "|dx|" << std::endl;
    std::cout << detx << std::endl;
  std::cout << "|dy|" << std::endl;
    std::cout << dety << std::endl;
  std::cout << "|dz|" << std::endl;
    std::cout << detz << std::endl;
  std::cout << "|c|" << std::endl;
    std::cout << detc << std::endl;
  glm::vec3 center(detx/(2*deta),dety/(2*deta),detz/(2*deta));
  GLfloat r = std::sqrt(detx*detx+dety*dety+detz*detz - 4*deta*detc)/(2*std::abs(deta));
  Sphere * sphere = new Sphere(center,r);
  DebugUtilityFunctions::print(center);
  std::cout << r << std::endl;
  return sphere;
}

MeshAlgorithms::Tetrahedron::Tetrahedron(glm::vec4 vert1,glm::vec4 vert2, glm::vec4 vert3, glm::vec4 vert4) : vertices(glm::mat4(vert1,vert2,vert3,vert4)) {
  
}

MeshAlgorithms::Sphere::Sphere(glm::vec3 center,GLfloat radius) : center(center), radius(radius) {}

void MeshAlgorithms::delanay_triangulation(const std::vector<glm::vec4> & points, std::vector<Tetrahedron *> & triangulation,Algorithm algorithm) {
  switch(algorithm) {
  case BOWYERWATSON:
    bowyer_watson(points,triangulation);
    break;
  default:
    assert(0);
  }
}

void MeshAlgorithms::bowyer_watson(const std::vector<glm::vec4> & points, std::vector<Tetrahedron *> & triangulation) {
}

MeshAlgorithms::Tetrahedron * MeshAlgorithms::create_super_tetrahedron(const std::vector<glm::vec4>& points) {
  
}

void MeshAlgorithms::create_convex_hull_3d(const std::vector<glm::vec4> & points,std::vector<GLint> & convex_hull,Algorithm algorithm) {
  switch(algorithm) {
  case QUICKHULL:
    quickhull(points,convex_hull);
    break;
  default:
    assert(0);
  }
}

void MeshAlgorithms::quickhull(const std::vector<glm::vec4> & points, std::vector<GLint> & convex_hull) {
  //According to http://thomasdiewald.com/blog/?p=1888
  // Create initial simplex
  std::vector<GLint> extrema;
  find_extremum_points(points,extrema);
  std::vector<glm::vec4> extremum_point_values;
  for(GLuint ii=2; ii<extrema.size();ii++) {
    extremum_point_values.push_back(points[extrema[ii]]);
  }
  GLint third_point = farthest_point_to_line(extremum_point_values,points[extrema[0]],points[extrema[1]]);
  GLint fourth_point = farthest_point_to_triangle(points,glm::vec3(extremum_point_values[0]),glm::vec3(extremum_point_values[1]),glm::vec3(points[third_point]));

  glm::vec4 v1 = extremum_point_values[0];
  glm::vec4 v2 = extremum_point_values[1];
  glm::vec4 v3 = points[third_point];
  glm::vec4 v4 = points[fourth_point];

  GLint p1 = extrema[0];
  GLint p2 = extrema[1];
  GLint p3 = third_point;
  GLint p4 = fourth_point;

  Face * face_array[4];
  
  /*
  Face face1 = Face(p1,p2,p3,points);
  Face face2 = Face(p2,p4,p3,points);
  Face face3 = Face(p1,p3,p4,points);
  Face face4 = Face(p1,p4,p2,points);
  face_array[0] = &face1;
  face_array[1] = &face2;
  face_array[2] = &face3;
  face_array[3] = &face4;
  */

  Face * face1 = new Face(p1,p2,p3,points);
  Face * face2 = new Face(p2,p4,p3,points);
  Face * face3 = new Face(p1,p3,p4,points);
  Face * face4 = new Face(p1,p4,p2,points);
  face_array[0] = face1;
  face_array[1] = face2;
  face_array[2] = face3;
  face_array[3] = face4;
  //Face & face1_r = face1;
  if(glm::dot(glm::vec3(v4),face1->get_normal())>0) {
    for(GLint ii=0; ii<4;ii++) {
      face_array[ii]->swap_orientation();
    }
  }
  for(GLint ii=0; ii<4;ii++) {
    for(GLint jj=0;jj<4;jj++) {
      if(ii!=jj) {
	face_array[ii]->add_neighbour(face_array[jj]);
      }
    }
  }
  
  //Face face1(extrema[0],extrema[1],third_point,points);
  //Check the correct orientation
  //Face face1(extrema[0],extrema[1],third_point,points);
  
  //Face face1(extremum_point_values[0],extremum_point_values[1],points[third_point]);
  //Face face2(extremum_point_values[0],extremum_point_values[1],points[third_point]);
  
  std::vector<GLint> vector_array[4];
  GLint np = points.size();
  for(GLint ii=0;ii<np;ii++) {
    for(GLint jj=0;jj<4;jj++) {
      if(glm::dot(glm::vec3(points[ii]),face_array[jj]->get_normal())>=0) {
	vector_array[jj].push_back(ii);
	break;
      }
    }
  }

  std::stack<Face *> face_stack;
  //std::vector<Face *> face_vector;
  std::unordered_map<Face *,std::vector<GLint>*> point_set_map;

  for(GLint ii=0;ii<4;ii++) {
    if(vector_array[ii].size() != 0) {
      face_stack.push(face_array[ii]);
      //face_vector.push_back(
      point_set_map[face_array[ii]] = &vector_array[ii];
    }
  }
  assert(face_stack.size());

  while(face_stack.size() > 0) {
    Face * face = face_stack.top();
    face_stack.pop();
    if(point_set_map.find(face) == point_set_map.end() || point_set_map.empty()) {
      continue;
    }

    std::vector<GLint> * point_set = point_set_map[face];
    std::vector<glm::vec4> point_set_values;
    GLint ps_size = point_set->size();
    point_set_values.reserve(ps_size);
    for(GLint ii=0;ii<ps_size;ii++) { // This is very slow. Should be changed so that there is no copying.
      point_set_values.push_back(points[(*point_set)[ii]]);
    }
    GLint farthest_point = farthest_point_to_triangle(point_set_values,glm::vec3(face->get_v1()),glm::vec3(face->get_v2()),glm::vec3(face->get_v3()));

    std::queue<Face *> light_faces;
    std::unordered_set<Face *> visited_faces;
    light_faces.push(face);
    visited_faces.insert(face);
    find_light_faces(light_faces,visited_faces,face,farthest_point,points);
    std::cout << "lf size: " << light_faces.size() << std::endl;
    // Next find the convex horizon.
    // First step is to project the system to a 2d subspace spanned by two vectors normal to farthest_point and some point on one of the light_faces.
    glm::vec3 u;
    glm::vec3 v;
    create_orthogonal_basis(glm::vec3(points[farthest_point] - light_faces.front()->get_v1()),u,v);
    std::vector<glm::vec2> light_face_values;
    std::vector<GLint> light_face_points;
    std::unordered_set<GLint> used_light_face_points;
    while(!light_faces.empty()) {
      Face * current = light_faces.front();
      light_faces.pop();
      for(GLint jj=0; jj<3;jj++) {
	GLint vertex = current->get_vertex(jj);
	if(used_light_face_points.find(vertex) == used_light_face_points.end()) {
	  used_light_face_points.insert(vertex);
	  light_face_values.push_back(glm::vec2(glm::dot(glm::vec3(points[vertex]),u),glm::dot(glm::vec3(points[vertex]),v)));
	  light_face_points.push_back(vertex);
	}
      }
    }
    std::vector<GLint> horizon_convex_hull;
    create_convex_hull_2d(light_face_values,horizon_convex_hull);
    std::vector<GLint> horizon_edges;
    GLint horizon_edges_size = horizon_convex_hull.size();
    horizon_edges.reserve(horizon_edges_size);
    for(GLint ii=0; ii<horizon_edges_size;ii++) {
      horizon_edges.push_back(light_face_points[horizon_convex_hull[ii]]);
    }
    for(GLint ii=0;ii<horizon_edges_size;ii++) {
      Face * horizon_face = new Face(farthest_point,horizon_edges[ii],horizon_edges[(ii+1) % horizon_edges.size()],points);
      
    }
  }

  
  
  // This is for debugging

  convex_hull.push_back(p1);
  convex_hull.push_back(p2);
  convex_hull.push_back(p3);
  convex_hull.push_back(p4);
}

void MeshAlgorithms::find_extremum_points(const std::vector<glm::vec4> & points, std::vector<GLint> & extrema) {
  GLuint N = points.size();
  extrema.reserve(6);
  GLfloat min_val[3];
  GLfloat max_val[3];
  for(GLuint ii=0;ii<3;ii++) {
    min_val[ii] = std::numeric_limits<GLfloat>::max();
    max_val[ii] = -std::numeric_limits<GLfloat>::max();
  }
  for(GLuint ii=0; ii<N;ii++) {
    for(GLuint jj=0;jj<3;jj++) {
      if(points[ii][jj]<min_val[jj]) {
	min_val[jj] = points[ii][jj];
	extrema.insert(extrema.begin()+jj*2,ii);
      }
      if(points[ii][jj]>max_val[jj]) {
	max_val[jj] = points[ii][jj];
	extrema.insert(extrema.begin()+jj*2+1,ii);
      }
    }
  }
}

GLint MeshAlgorithms::farthest_point_to_line(const std::vector<glm::vec4> & points,glm::vec4 line_vert1,glm::vec4 line_vert2) {
  glm::vec3 v1(line_vert1);
  glm::vec3 v2(line_vert2);
  GLuint N = points.size();
  GLfloat max_distance2 = 0.0f;
  GLint largest_point = -1;
  for(GLuint ii=0;ii<N;ii++) {
    GLfloat distance2 = glm::length2(glm::cross(v2-v1,v1-glm::vec3(points[ii])))/glm::distance2(v2,v1);
    if(distance2 > max_distance2) {
      max_distance2 = distance2;
      largest_point = ii;
    }
  }
  assert(largest_point >= 0);
  return largest_point;
}
/*
GLint MeshAlgorithms::farthest_point_to_triangle(const std::vector<glm::vec4> & points,glm::vec4 vert1,glm::vec4 vert2,glm::vec4 vert3) {
  glm::vec3 v1(vert1);
  glm::vec3 v2(vert2);
  glm::vec3 v3(vert3);
  glm::vec3 normal = glm::normalize(glm::cross(v2-v1,v3-v1));
  GLuint N=points.size();
  GLfloat max_distance = 0.0f;
  for(GLuint ii=0;ii<N;ii++) {
    glm::vec3 cp(points[ii]);
    glm::vec3 v2cp = cp-v1;
    //Projection to triangle
    GLfloat distance = glm::dot(v2cp,normal);
    glm::vec3 projected_point = v2cp - distance*normal;
    
  }
  
}
*/

 GLint MeshAlgorithms::farthest_point_to_triangle(const std::vector<glm::vec4> & points,const glm::vec3 & vert1,const glm::vec3 & vert2,const glm::vec3 & vert3) {
   // Using http://www.geometrictools.com/Documentation/DistancePoint3Triangle3.pdf
   glm::vec3 B = vert1;
   glm::vec3 E0 = vert2 - vert1;
   glm::vec3 E1 = vert3 - vert1;

   //glm::vec3 P = glm::vec3(points[0]);
  

   GLfloat a = glm::dot(E0,E0);
   GLfloat b = glm::dot(E0,E1);
   GLfloat c = glm::dot(E1,E1);
   GLfloat det = a*c - b*b;

   GLfloat max_distance2 = -1.0;
   GLint farthest_point = -1;
  
   for(GLuint ii=0; ii<points.size();ii++) {
     glm::vec3 P = glm::vec3(points[ii]);

     glm::vec3 D = B - P;
     GLfloat d = glm::dot(E0,D);
     GLfloat e = glm::dot(E1,D);
     //GLfloat f = glm::dot(D,D);

     GLfloat s = b*e - c*d;
     GLfloat t = b*d - a*e;
     /*
       ^ t
       \2|
       \|
       |  
       |\   1
       3  | \
       |0 \
       -----------------> s
       4  | 5 \  6

     */
     if(s+t <= det) {
       if(s < 0) {
	 if(t < 0) {
	   //region 4
	   s = 0;
	   t = 0;
	   /*
	     GLfloat tmp0 = b+d;
	     GLfloat tmp1 = c+e;
	     if(tmp1 > tmp0) {
	     GLfloat numer = tmp1 - tmp0;
	     GLfloat denom = a-2*b+c;
	     s = (numer >= denom ? 1 : numer/denom);
	     t = 1-s;
	     } else {
	     s = 0;
	     t = (tmp1 <= 0 ? 1 : (e >= 0 ? 0 : -e/c));
	     }
	   */
	 } else {
	   //region 3
	   s = 0;
	   t = (e >= 0 ? 0 : ( -e >= c ? 1 : -e/c));
	 }
       } else {
	 if(t < 0) {
	   //region 5
	   t = 0;
	   s = (d >= 0 ? 0 : ( -d >= a ? 1 : -d/a));
	 } else {
	   // region 0
	   // Point inside triangle, just scale by inverse of determinant
	   GLfloat inv_det = 1.0f/det;
	   s *= inv_det;
	   t *= inv_det;
	 }
       }
     } else {
       if(s < 0) {
	 //region 2
	 GLfloat tmp0 = b+d;
	 GLfloat tmp1 = c+e;
	 if(tmp1 > tmp0) {
	   GLfloat numer = tmp1 - tmp0;
	   GLfloat denom = a-2*b+c;
	   s = (numer >= denom ? 1 : numer/denom);
	   t = 1-s;
	 } else {
	   s = 0;
	   t = (tmp1 <= 0 ? 1 : (e >= 0 ? 0 : -e/c));
	 }
       } else {
	 if(t < 0) {
	   //region 6
	   s = 1;
	   t = 0;
	   /*
	     GLfloat tmp0 = b+d;
	     GLfloat tmp1 = c+e;
	     if(tmp1 > tmp0) {
	     GLfloat numer = tmp1 - tmp0;
	     GLfloat denom = a-2*b+c;
	     s = (numer >= denom ? 1 : numer/denom);
	     t = 1-s;
	     } else {
	     s = 0;
	     t = (tmp1 <= 0 ? 1 : (e >= 0 ? 0 : -e/c));
	     }
	   */
	 } else {
	   //region 1
	   GLfloat numer = c+e-b-d;
	   if(numer <= 0) {
	     s = 0;
	   } else {
	     GLfloat denom = a-2*b+c;
	     s = (numer >= denom ? 1 : numer/denom);
	   }
	   t = 1-s;
	 }
       }
     }
     glm::vec3 point_on_triangle = B + s*E0 + t*E1;
     GLfloat distance2 = glm::distance2(point_on_triangle,P)/std::pow(points[ii].w,2);

     if(distance2 > max_distance2) {
       max_distance2 = distance2;
       farthest_point = ii;
     }
   }
  return farthest_point;
}

 GLfloat MeshAlgorithms::distance2_to_triangle(const glm::vec4 & P, const glm::vec3 & vert1,const glm::vec3 & vert2, const glm::vec3 & vert3, glm::vec3 & point_on_triangle) {
     // Using http://www.geometrictools.com/Documentation/DistancePoint3Triangle3.pdf
  glm::vec3 v1(vert1);
  glm::vec3 v2(vert2);
  glm::vec3 v3(vert3);

  glm::vec3 B = v1;
  glm::vec3 E0 = v2 - v1;
  glm::vec3 E1 = v3 - v1;

  //glm::vec3 P = glm::vec3(points[0]);
  
  glm::vec3 D = B - glm::vec3(P);
  GLfloat a = glm::dot(E0,E0);
  GLfloat b = glm::dot(E0,E1);
  GLfloat c = glm::dot(E1,E1);
  GLfloat d = glm::dot(E0,D);
  GLfloat e = glm::dot(E1,D);
  GLfloat f = glm::dot(D,D);

  GLfloat det = a*c - b*b;
  GLfloat s = b*e - c*d;
  GLfloat t = b*d - a*e;
  /*
      ^ t
    \2|
     \|
      |  
      |\   1
   3  | \
      |0 \
-----------------> s
   4  | 5 \  6

  */
  if(s+t <= det) {
    if(s < 0) {
      if(t < 0) {
	//region 4
	s = 0;
	t = 0;
	/*
	GLfloat tmp0 = b+d;
	GLfloat tmp1 = c+e;
	if(tmp1 > tmp0) {
	  GLfloat numer = tmp1 - tmp0;
	  GLfloat denom = a-2*b+c;
	  s = (numer >= denom ? 1 : numer/denom);
	  t = 1-s;
	} else {
	  s = 0;
	  t = (tmp1 <= 0 ? 1 : (e >= 0 ? 0 : -e/c));
	}
	*/
      } else {
	//region 3
	s = 0;
	t = (e >= 0 ? 0 : ( -e >= c ? 1 : -e/c));
      }
    } else {
      if(t < 0) {
	//region 5
	t = 0;
	s = (d >= 0 ? 0 : ( -d >= a ? 1 : -d/a));
      } else {
	// region 0
	// Point inside triangle, just scale by inverse of determinant
	GLfloat inv_det = 1.0f/det;
	s *= inv_det;
	t *= inv_det;
      }
    }
  } else {
    if(s < 0) {
      //region 2
      GLfloat tmp0 = b+d;
      GLfloat tmp1 = c+e;
      if(tmp1 > tmp0) {
	GLfloat numer = tmp1 - tmp0;
	GLfloat denom = a-2*b+c;
	s = (numer >= denom ? 1 : numer/denom);
	t = 1-s;
      } else {
	s = 0;
	t = (tmp1 <= 0 ? 1 : (e >= 0 ? 0 : -e/c));
      }
    } else {
      if(t < 0) {
	//region 6
	s = 1;
	t = 0;
	/*
      GLfloat tmp0 = b+d;
      GLfloat tmp1 = c+e;
      if(tmp1 > tmp0) {
	GLfloat numer = tmp1 - tmp0;
	GLfloat denom = a-2*b+c;
	s = (numer >= denom ? 1 : numer/denom);
	t = 1-s;
      } else {
	s = 0;
	t = (tmp1 <= 0 ? 1 : (e >= 0 ? 0 : -e/c));
      }
	*/
      } else {
	//region 1
	GLfloat numer = c+e-b-d;
	if(numer <= 0) {
	  s = 0;
	} else {
	  GLfloat denom = a-2*b+c;
	  s = (numer >= denom ? 1 : numer/denom);
	}
	t = 1-s;
      }
    }
  }
  point_on_triangle = B + s*E0 + t*E1;
  GLfloat distance2 = glm::distance2(point_on_triangle,glm::vec3(P))/std::pow(P.w,2);
  return distance2;
 }

MeshAlgorithms::Face::Face(GLint p1,GLint p2,GLint p3,const std::vector<glm::vec4> & points) : p1(p1),p2(p2),p3(p3),points(points) {
  normal_direction = glm::cross(glm::vec3(points[p2] - points[p1]),glm::vec3(points[p3]-points[p1]));
}

MeshAlgorithms::Face::Face(GLint p1,GLint p2,GLint p3,const std::vector<glm::vec4> & points,glm::vec3 normal) : p1(p1),p2(p2),p3(p3),points(points),normal_direction(normal) {}

void MeshAlgorithms::Face::swap_orientation() {
  normal_direction = -normal_direction;
  GLint temp = p2;
  p2 = p3;
  p3 = temp;
}

/*
  Creates an orthogonal basis using vector v1 as a starting point. v2 and v3 will be normalized, but still the basis is orthonormal only if |v1| = 1. Should be numerically quite robust.
 */

void MeshAlgorithms::create_orthogonal_basis(const glm::vec3 & v1, glm::vec3 & v2,glm::vec3 & v3) {
  GLfloat min = std::numeric_limits<GLfloat>::max();
  GLint min_index;
  for(GLint ii=0;ii<3;ii++) {
    if(std::abs(v1[ii])<min) {
      min = std::abs(v1[ii]);
      min_index = ii;
    }
  }
  glm::vec3 w(0,0,0);
  w[min_index] = 1.0f;
  v2 = glm::normalize(glm::cross(v1,w));
  v3 = glm::normalize(glm::cross(v1,v2));
  /*
  GLfloat uu = glm::dot(v1,v1);
  if(uu == 0.0f) {
    v2 = glm::vec3(0,0,0);
    v3 = glm::vec3(0,0,0);
    return;
  }
  GLfloat uv = glm::dot(v
  */
}

void MeshAlgorithms::find_light_faces(std::queue<Face *> & light_faces,std::unordered_set<Face *> & visited_faces,Face * face,GLint farthest_point,const std::vector<glm::vec4> & points) {
  if((visited_faces.find(face) == visited_faces.end()) && glm::dot(glm::vec3(points[farthest_point]),face->get_normal())>0) { // Light face
    light_faces.push(face);
    visited_faces.insert(face);
    std::unordered_set<Face *> * neighbours = face->get_neighbours();
    for(auto neighbour : *neighbours) {
      find_light_faces(light_faces,visited_faces,neighbour,farthest_point,points);
    }

  }
}

void MeshAlgorithms::create_convex_hull_2d(const std::vector<glm::vec2> & points,std::vector<GLint> & convex_hull,Algorithm algorithm) {
    switch(algorithm) {
  case QUICKHULL:
    quickhull(points,convex_hull);
    break;
    case JARVISMARCH:
      jarvis_march(points,convex_hull);
      break;
  default:
    assert(0);
  }
}

void MeshAlgorithms::quickhull(const std::vector<glm::vec2> & points, std::vector<GLint> & convex_hull){
  assert(0); // NOT DONE
}



 GLint MeshAlgorithms::Face::get_vertex(GLint number) const {
   switch(number) {
   case 0:
     return p1;
   case 1:
     return p2;
   case 2:
     return p3;
   default:
     return -1;
   }
 }

void MeshAlgorithms::jarvis_march(const std::vector<glm::vec2> & points,std::vector<GLint> & convex_hull) {
  //pointOnHull = leftmost point in S
  GLint point_on_hull;
  GLint N = points.size();
  GLfloat min = std::numeric_limits<GLfloat>::max();
  for(GLint ii= 0; ii < N; ii++) {
    if(points[ii].x < min) {
      min = points[ii].x;
      point_on_hull = ii;
    }
  }
  GLint ii = 0;
  GLint endpoint;
  do {
    convex_hull.push_back(point_on_hull); 
    endpoint = 0;
    for(GLint jj=1;jj<N;jj++) {
      if((endpoint == point_on_hull) || left_of_line(points[jj],points[convex_hull[ii]],points[endpoint])) {
	endpoint = jj;
      }
    }
    ii++;
    point_on_hull = endpoint;	
  } while(endpoint != convex_hull[0]);
  /*
   repeat
      P[i] = pointOnHull
      endpoint = S[0]         // initial endpoint for a candidate edge on the hull
      for j from 1 to |S|
         if (endpoint == pointOnHull) or (S[j] is on left of line from P[i] to endpoint)
            endpoint = S[j]   // found greater left turn, update endpoint
      i = i+1
      pointOnHull = endpoint
   until endpoint == P[0]      // wrapped around to first hull point
  */


}

GLboolean MeshAlgorithms::left_of_line(const glm::vec2 & p,const glm::vec2 & v1,const glm::vec2 & v2) {
  //position = sign( (Bx-Ax)*(Y-Ay) - (By-Ay)*(X-Ax) )
  GLfloat determinant = (v2.x-v1.x)*(p.y-v1.y) - (v2.y-v1.y)*(p.x-v1.x);
  //GLint sign = (determinant > 0) - (determinant < 0);
  return (determinant > 0) ? GL_TRUE : ((determinant < 0) ? GL_FALSE : GL_FALSE);
    //return sign;
}
