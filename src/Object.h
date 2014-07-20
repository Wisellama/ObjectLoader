/**
   Object - a storage class for a .obj file

   Uses Triangle.h to keep track of faces.
   Keeps a list of vertices and vertex normals here in the Object. 

   Shader attribute order in the Vertex Array:
   (number of values in parentheses)
   position 3
   normal 3
   ambient 3
   diffuse 3
   specular 3
   shininess 1
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Triangle.h"
#include "Shader.h"
#include "utilities.h"

using namespace std;

class Object {
public:
  Object();
  Object(string filename, Shader* s);

  void outputToObj(string filename) const;
  void parseObjectFile(string filename);

  void glDraw();
  void setShader(Shader* shader);
  void openglInitialization();
  GLuint getVao() const;

  glm::mat4 getTransform() const;
  void setTransform(glm::mat4 t);
  void applyModelMatrix() const;

  void setAmbientColor(glm::vec3 c);
  void setDiffuseColor(glm::vec3 c);
  void setSpecularColor(glm::vec3 c);
  void setShininess(float s);


private:
  vector<Triangle> triangles;
  vector<glm::vec3> vertices;
  vector<glm::vec3> vertexNormals;
  glm::mat4 transformation;
  Shader* shader;
  GLuint vao;
  
  // shader elements
  uint elms; // total number of elements per vertex
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;

  int init(string filename, Shader* s);

  int parseOBJVertex(string s) const;
  int parseOBJVertexNormal(string s) const;
  
  glm::vec3 surfaceNormal(Triangle t) const;
};

#endif

