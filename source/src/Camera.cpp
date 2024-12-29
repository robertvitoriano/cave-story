#include "Camera.h"

Camera &Camera::getInstance()
{
  static Camera instance;
  return instance;
}
Camera::Camera() : _width(globals::SCREEN_WIDTH), _height(globals::SCREEN_HEIGHT), _speed(2), _center({globals::SCREEN_WIDTH / 2, globals::SCREEN_HEIGHT / 2})
{
}

int Camera::getWidth()
{
  return this->_width;
}

int Camera::getHeight()
{
  return this->_height;
}

Vector2 Camera::getCenter()
{
  return this->_center;
}
