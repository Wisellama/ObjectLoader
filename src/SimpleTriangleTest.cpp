/**
   SimpleTriangleTest - a simple test case to make sure everything 
   is working correctly. Draws a rectangle using 2 triangles.
 */

#include "SimpleTriangleTest.h"

SimpleTriangleTest::SimpleTriangleTest() {

}

void SimpleTriangleTest::initialize(Shader* s) {
  GLuint shaderProgram = s->getProgram();

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);

  GLfloat vertices[] = {
    -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Top-left
    0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Top-right
    0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom-right
    -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f  // Bottom-left
  };

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, 
               GL_STATIC_DRAW);

  GLuint ebo;
  glGenBuffers(1, &ebo);

  GLuint elements[] = {
    0, 1, 2,
    2, 3, 0
  };

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), 
               elements, GL_STATIC_DRAW);

  glUseProgram(shaderProgram);

  GLint posAttrib = glGetAttribLocation(shaderProgram, "vPosition");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 
                        6*sizeof(GLfloat), 0);

  GLint color = glGetAttribLocation(shaderProgram, "diffuseColor");
  glEnableVertexAttribArray(color);
  glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 
                        6*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
  
}

void SimpleTriangleTest::glDraw() {
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
