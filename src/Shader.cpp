/**
   Shader - a storage class for the Vertex and Fragment Shader
 */

#include "Shader.h"

using namespace std;

int Shader::init(string vertFile, string fragFile) {
  GLint successful = GL_TRUE;
  string vertStr = this->loadSource(vertFile);
  string fragStr = this->loadSource(fragFile);
  const char* vertSource = vertStr.c_str();
  const char* fragSource = fragStr.c_str();

  // create the shaders
  this->vert = glCreateShader(GL_VERTEX_SHADER);
  this->frag = glCreateShader(GL_FRAGMENT_SHADER);

  // give opengl the sources
  glShaderSource(this->vert, 1, &vertSource, NULL);
  glShaderSource(this->frag, 1, &fragSource, NULL);

  // compile them
  glCompileShader(this->vert);
  glCompileShader(this->frag);

  // check for errors
  GLint status;
  glGetShaderiv(this->vert, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    cerr << "Failed to compile the vertex shader\n\n";
    char buffer[512];
    glGetShaderInfoLog(this->vert, 512, NULL, buffer);
    cerr << buffer << "\n\n";
    successful = status;
  }
  status = GL_FALSE;
  glGetShaderiv(this->frag, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    cerr << "Failed to compile the fragment shader\n\n";
    char buffer[512];
    glGetShaderInfoLog(this->frag, 512, NULL, buffer);
    cerr << buffer << "\n\n";
    successful = status;
  }

  GLuint shaderProgram = glCreateProgram();
  this->program = shaderProgram;

  glAttachShader(shaderProgram, this->vert);
  glAttachShader(shaderProgram, this->frag);
  
  // links up with the variable "outColor" in the shader source
  glBindFragDataLocation(shaderProgram, 0, "outColor");

  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  return successful;
}

Shader::Shader(string vertFile, string fragFile) {
  init(vertFile, fragFile);
}

Shader::Shader() {
  this->vert = -1;
  this->frag = -1;
  this->program = -1;
}

GLuint Shader::getVert() const {
  return this->vert;
}

GLuint Shader::getFrag() const {
  return this->frag;
}

GLuint Shader::getProgram() const {
  return this->program;
}



/**
   Private
 */
string Shader::loadSource(string file) {
  ifstream shaderFile (file.c_str());

  string line;
  stringstream ss;
  if (shaderFile.is_open()) {
    while (getline(shaderFile, line)) {
      ss << line << "\n";
    }
  }

  return ss.str();
}

