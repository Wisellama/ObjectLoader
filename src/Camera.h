/**
   Camera - storage class for the viewer's camera

   Contains position, viewing direction, and up direction.
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

using namespace glm;

class Camera {
 public:
  Camera();
  Camera(vec3 pos, vec3 dir, vec3 up);

  vec3 getPosition() const;
  vec3 getTarget() const;
  vec3 getViewDir() const;
  vec3 getUpDir() const;
  
  void translate(vec3 v);
  void rotate(vec3 v);

 private:
  vec3 position;
  vec3 target;
  vec3 up;

  int init(vec3 pos, vec3 dir, vec3 up);
};

#endif
