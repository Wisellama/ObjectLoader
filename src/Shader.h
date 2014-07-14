/**
   Shader - a storage class for an OpenGL shader
 */
#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Shader {
 public:
  Shader();
  Shader(string vertFile, string fragFile);
  GLuint getVert() const;
  GLuint getFrag() const;
  GLuint getProgram() const;

 private:
  GLuint vert,frag,program;

  int init(string vertFile, string fragFile);
  string loadSource(string file);
};

#endif
