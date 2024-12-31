#include "Camera.h"

Camera &Camera::getInstance()
{
  static Camera instance;
  return instance;
}
Camera::Camera() : _width(globals::SCREEN_WIDTH), _height(globals::SCREEN_HEIGHT), _speed(2), _center({globals::SCREEN_WIDTH / 2, globals::SCREEN_HEIGHT / 2}), _rightLimit(globals::SCREEN_WIDTH - 100)
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
void Camera::setCenter(Vector2 position)
{
  this->_center = position;
}
int Camera::getRightLimit()
{
  return this->_rightLimit;
}

void Camera::drawDebug(SDL_Renderer *renderer)
{
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

  SDL_Rect centerRect = {
      static_cast<int>(this->_center.x - 2),
      static_cast<int>(this->_center.y - 2),
      4,
      4};
  SDL_RenderFillRect(renderer, &centerRect);

  SDL_RenderDrawLine(renderer,
                     this->_rightLimit,
                     0,
                     this->_rightLimit,
                     this->_height);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
}
