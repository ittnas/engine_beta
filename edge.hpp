#ifndef EDGE_HPP
#define EDGE_HPP

#include <assert.h>

class Edge {
private:
  GLuint vert1;
  GLuint vert2;
public:
  Edge(GLuint vertex1, GLuint vertex2) {
    assert(vertex1 != vertex2);
    if(vertex1 <  vertex2) {
      vert1 = vertex1;
      vert2 = vertex2;
    } else {
      vert1 = vertex2;
      vert2 = vertex1;
    }
  }
  GLuint get_vertex1() const {
    return vert1;
  }
  GLuint get_vertex2() const {
    return vert2;
  }
};

class Neighbours {
private:
  GLuint face1;
  GLuint face2;
public:
  Neighbours() {
    face1 = face2 = (GLuint)-1;
    
  }


  void add_neighbour(GLuint face_index) {
    if(face1 == -1) {
      face1 = face_index;
    } else if (face2 == -1) {
      face2 = face_index;
    } else {
      assert(0); //We shold never be here
    }
  }

  GLuint get_other(GLuint me) const {
    if(face1 == me) {
      return face2;
    } else if (face2 == me) {
      return face1;
    } else {
      assert(0);
    }
  }
};

class ComparaEdges {
public:
  GLboolean operator()(const Edge & edge1, const Edge & edge2) {
    if(edge1.get_vertex1() < edge2.get_vertex1()) {
      return true;
    } else if(edge1.get_vertex1() == edge2.get_vertex1()) {
      return (edge1.get_vertex2() < edge2.get_vertex2());
    } else {
      return false;
    }
  }
};

#endif
