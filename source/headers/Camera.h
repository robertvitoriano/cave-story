#ifndef CAMERA_H
#define CAMERA_H

#include "globals.h"

class Camera
{
public:
  Camera(const Camera &) = delete;
  Camera &operator=(const Camera &) = delete;

  static Camera &getInstance();

  int getWidth();
  int getHeight();
  Vector2 getCenter();

private:
  Camera();

  int _speed;
  int _width;
  int _height;
  Vector2 _center;
};

#endif
