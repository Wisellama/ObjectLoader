/**
   Triangle - a storage class for a triangle face from a .obj file
 */

#include "Triangle.h"

int Triangle::init(uint a, uint b, uint c,    
                   uint an, uint bn, uint cn) {
  std::vector<uint> v, vn;

  // vertex indices
  v.push_back(a);
  v.push_back(b);
  v.push_back(c);
  // vertex normal indices
  vn.push_back(an);
  vn.push_back(bn);
  vn.push_back(cn);

  this->vertices = v;
  this->vertexNormals = vn;

  return 0;
}

Triangle::Triangle() {
  init(0,0,0,0,0,0);
}

Triangle::Triangle(uint a, uint b, uint c,    
                   uint an, uint bn, uint cn) {
  init(a,b,c,an,bn,cn);
}

Triangle::Triangle(uint a, uint b, uint c) {
  init(a,b,c,0,0,0);
}

std::vector<uint> Triangle::getVertices() const {
  return this->vertices;
}

std::vector<uint> Triangle::getVertexNormals() const {
  return this->vertexNormals;
}

