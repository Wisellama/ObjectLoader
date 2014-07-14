/**
   SimpleTriangleTest - a simple test case to make sure everything 
   is working correctly. Draws a rectangle using 2 triangles.
*/

#ifndef SIMPLETRIANGLETEST_H
#define SIMPLETRIANGLETEST_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

class SimpleTriangleTest {
 public:
SimpleTriangleTest ();
  void initialize(Shader* s);
  void glDraw();
 private:

};

#endif
