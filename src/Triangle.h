/**
   Triangle - a storage class for a triangle face from a .obj file
 */

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>

typedef unsigned int uint;

class Triangle {
 public:
  Triangle();
  Triangle(uint a, uint b, uint c,    
           uint an, uint bn, uint cn);
  Triangle(uint a, uint b, uint c);

  std::vector<uint> getVertices() const;
  std::vector<uint> getVertexNormals() const;

 private:
  std::vector<uint> vertices, vertexNormals; 
  
  int init(uint a, uint b, uint c,    
           uint an, uint bn, uint cn);
};

#endif
