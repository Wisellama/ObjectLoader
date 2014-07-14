/**
   Triangle - a storage class for a triangle
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

  void setElementBase(uint e);
  uint getElementBase() const;
  bool elementIsSet() const;
  
 private:
  std::vector<uint> vertices, vertexNormals; 
  
  // This is the element index for the first vertex.
  // The other two are e+1 and e+2 respectively
  uint element;
  bool elementWasSet;
  
  int init(uint a, uint b, uint c,    
           uint an, uint bn, uint cn);
};

#endif
