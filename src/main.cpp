#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Camera.h"
#include "Shader.h"
#include "Object.h"
#include "utilities.h"

using namespace std;

/**
   Some global storage classes
 */
class Viewport {
public:
  uint width, height;
  float fov, near, far;
};
Viewport viewport;

class Globals {
public:
  vector<Object> objects;
  Camera camera;
  float translate_step, rotate_step;
  double mousex, mousey;
  GLFWwindow* window;
  Shader shader;
};
Globals globals;

/**
   and the global initializations
 */
void initGlobals() {
  // viewport
  viewport.width = 800;
  viewport.height = 600;
  viewport.near = 0.1f;
  viewport.far = 100;
  viewport.fov = 45.0f;

  // globals
  globals.translate_step = 0.05f;
  globals.rotate_step = 0.01f;
  globals.mousex = viewport.width/2.0;
  globals.mousey = viewport.height/2.0;
}

// pre-declaration
void updateViewMatrix();

void drawScene() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  updateViewMatrix();

  for (uint i = 0; i < globals.objects.size(); i++) {
    globals.objects[i].glDraw();
  }
}

void updateViewMatrix() {
  GLuint shaderProgram = globals.shader.getProgram();
  glm::mat4 view = glm::lookAt
    (globals.camera.getPosition(),
     globals.camera.getTarget(), // the position to look at
     globals.camera.getUpDir());
  GLint uniView = glGetUniformLocation(shaderProgram, "view");
  glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
}

void initializeMatrices() {
  GLuint shaderProgram = globals.shader.getProgram();

  glm::mat4 model;
  // example
  //model = glm::rotate(model, PI, glm::vec3(0.0f,0.0f,1.0f));

  GLint uniModel = glGetUniformLocation(shaderProgram, "model");
  // (var, # of matrices, transpose, matrix as an array)
  glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

  glm::mat4 view = glm::lookAt
    (globals.camera.getPosition(),
     globals.camera.getTarget(), // the position to look at
     globals.camera.getUpDir());
  GLint uniView = glGetUniformLocation(shaderProgram, "view");
  glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

  glm::mat4 proj = 
    glm::perspective(viewport.fov,1.0f*viewport.width/viewport.height, 
                     viewport.near, viewport.far);
  GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
  glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
  
  
}

void moveCamera() {
  float r = globals.rotate_step;
  float t = globals.translate_step;
  glm::vec3 up = globals.camera.getUpDir();
  glm::vec3 forward = globals.camera.getViewDir();
  glm::vec3 right = glm::cross(forward,up);

  float epsilon = 0.001f;
  float scale = 2000.0f;

  // mouse
  double xpos, ypos;
  double xcenter = viewport.width/2.0;
  double ycenter = viewport.height/2.0;
  glfwGetCursorPos(globals.window, &xpos, &ypos);
  float diffx = (xcenter-xpos);
  float diffy = (ycenter-ypos);

  if (abs(diffy) > epsilon) {
    globals.camera.rotate(diffy/scale*right);
  }
  if (abs(diffx) > epsilon) {
    globals.camera.rotate(diffx/scale*up);
  }

  glfwSetCursorPos(globals.window, xcenter, ycenter);


  // keyboard
  if (glfwGetKey(globals.window, 'A') == GLFW_PRESS) {
    globals.camera.translate(-t*right);
  }
  if (glfwGetKey(globals.window, 'D') == GLFW_PRESS) {
    globals.camera.translate(t*right);
  }
  if (glfwGetKey(globals.window, 'W') == GLFW_PRESS) {
    globals.camera.translate(t*forward);
  }
  if (glfwGetKey(globals.window, 'S') == GLFW_PRESS) {
    globals.camera.translate(-t*forward);
  }
  if (glfwGetKey(globals.window, 'Q') == GLFW_PRESS) {
    globals.camera.rotate(-r*forward);
  }
  if (glfwGetKey(globals.window, 'E') == GLFW_PRESS) {
    globals.camera.rotate(r*forward);
  }
}

GLFWwindow* initializeGlfw() {
  /**
     Basic GLFW initialization for OpenGL 3.2+ only 
     with no backwards compatibility. 
   */
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  // windowed
  GLFWwindow* window = 
    glfwCreateWindow(viewport.width, viewport.height, 
                     "Object Loader", NULL, NULL);

  // fullscreen
  /*GLFWwindow* window = 
    glfwCreateWindow(800, 600, "OpenGL", 
    glfwGetPrimaryMonitor(), NULL); */

  glfwMakeContextCurrent(window);
  return window;
}

int main(int argc, char* argv[]) {
  // initialize the global settings
  initGlobals();

  // set up GLFW
  GLFWwindow* window = initializeGlfw();
  globals.window = window;

  // GLEW setup
  glewExperimental = GL_TRUE;
  glewInit();

  // everything currently uses the same shader
  Shader shader ("shaders/vertexShader.c", "shaders/fragmentShader.c");
  globals.shader = shader;
  
  // parse args
  int i;
  for (i = 1; i < argc; i++) {
    string s (argv[i]);
    Object obj (s, &globals.shader);
    globals.objects.push_back(obj);
  }
  i--;
  cout << "Loaded " << i << " ";
  if (i > 1) {
    cout << "objects ";
  } else {
    cout << "object ";
  }
  cout << "successfully\n";

  initializeMatrices();

  glEnable(GL_DEPTH_TEST);

  /**
     the main, infinite loop
   */
  while(!glfwWindowShouldClose(window)) {
    glfwSwapBuffers(window);
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GL_TRUE);
      cout << "Quitting\n\n";
    }

    moveCamera();
    drawScene();
  }

  glfwTerminate();
  return 0;
}
