/**
   Camera - storage class for the viewer's camera

   Contains position, viewing direction, and up direction.
 */

#include "Camera.h"

using namespace glm;

int Camera::init(vec3 pos, vec3 target, vec3 up) {
  this->position = pos;
  this->target = target;
  this->up = up;
  
  return 0;
}

Camera::Camera() {
  // default to looking down negative z-axis
  vec3 p (0.0f, 0.0f, 5.0f);
  vec3 t (0.0f, 0.0f, 0.0f);
  vec3 up (0.0f, 1.0f, 0.0f);
  init(p, t, up);
}

Camera::Camera(vec3 pos, vec3 dir, vec3 up) {
  init(pos, dir, up);
}

vec3 Camera::getPosition() const {
  return this->position;
}

vec3 Camera::getTarget() const {
  return this->target;
} 

vec3 Camera::getUpDir() const {
  return this->up;
}

vec3 Camera::getViewDir() const {
  return normalize(this->target - this->position);
}

void Camera::translate(vec3 v) {
  this->position += v;
  this->target += v; // keep the view direction constant
}

// axis-angle rotation
void Camera::rotate(vec3 v) {
  float angle = length(v);
  vec3 w = normalize(v);
  vec3 a = this->getViewDir();

  // rodrigues formula to rotate
  vec3 b = 
    a*cos(angle) + cross(w,a)*sin(angle) + w*dot(w,a)*(1-cos(angle));
  vec3 view = this->target - this->position;
  this->target = this->position + b*length(view);

  a = this->getUpDir();
  this->up = 
    a*cos(angle) + cross(w,a)*sin(angle) + w*dot(w,a)*(1-cos(angle));
}
