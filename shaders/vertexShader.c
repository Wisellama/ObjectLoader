/**
   vertexShader - an OpenGL shader that implements Phong shading.
*/

#version 150

// vertex properties
in vec3 vPosition;
in vec3 vNormal;
in vec3 ambientColor;
in vec3 diffuseColor;
in vec3 specularColor;
in float shininess;

// light properties
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 lightDirection; // if directional

// viewer properties
uniform vec3 viewPosition;
uniform vec3 viewDirection;

// matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

// outputs the color for this vertex
out vec4 Color;

void main() {
  /**
     Calculate the position after matrix transformations
  */
  mat4 modelView = view * model;
  mat3 normalMatrix;
  normalMatrix[0] = modelView[0].xyz;
  normalMatrix[1] = modelView[1].xyz;
  normalMatrix[2] = modelView[2].xyz;

  // output the position to the special gl_Position variable
  gl_Position = proj * modelView * vec4(vPosition, 1.0);


  /**
     Phong shading calculations for color
  */
  vec3 tempColor = vec3(0,0,0);
  vec3 d = lightDirection;
  vec3 n = vNormal;
  
  // point light needs to calculate direction
  if (lightDirection == vec3(0.0,0.0,0.0)) {
    d = normalize(vPosition - lightPosition);
  } 

  d = vec3(0,0,1);


  // ambient
  tempColor += ambientColor;

  // diffuse
  vec3 tempDiffColor = diffuseColor * dot(d, n);
  for (int i = 0; i < 3; i++) {
    if (tempDiffColor[i] < 0.0) {
      tempDiffColor[i] = 0.0;
    }
  }
  tempColor += tempDiffColor;

  // specular
  vec3 r = d + 2*(dot(-1*d,n)) * n;
  float re = dot(r,viewDirection);
  if (re < 0) {
    re = 0;
  }
  re = pow(re, shininess);
  
  vec3 tempSpecColor = specularColor * re;
  for (int i = 0; i < 3; i++) {
    if (tempSpecColor[i] < 0.0) {
      tempSpecColor[i] = 0.0;
    }
  }
  tempColor += tempSpecColor;

  // fix the color to fit constraints
  for (int i = 0; i < 3; i++) {
    if (tempColor[i] > 1) {
      tempColor[i] = 1;
    }
    if (tempColor[i] < 0) {
      tempColor[i] = 0;
    }
  }

  // output color
  Color = vec4(tempColor,1.0);
  //Color = vec4(1.0,1.0,1.0,1.0);
}
