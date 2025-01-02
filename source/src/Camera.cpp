#include "Camera.h"

Camera &Camera::getInstance()
{
  static Camera instance;
  return instance;
}
Camera::Camera() : Rectangle(0, 0, globals ::SCREEN_WIDTH,
                             globals::SCREEN_HEIGHT),
                   _speed(2), _center({globals::SCREEN_WIDTH / 2, globals::SCREEN_HEIGHT / 2}), _rightLimit(globals::SCREEN_WIDTH - 100)
{
  this->_offset = {0, 0};
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
                     this->getHeight());

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
}

void Camera::follow(Player *player, Level *level)
{
  this->_player = player;
  this->_level = level;
}
void Camera::update()
{
  if (!_player)
  {
    return;
  }
  float playerX = this->_player->getX();

  if (this->_level->isLevelWiderThanScreen())
  {
    this->handleScrollOffset(playerX);

    for (Tile &tile : this->_level->getTileList())
    {
      tile.setOffset(Vector2(-this->_offset.x, 0));
    }

    for (AnimatedTile &animatedTile : this->_level->getAnimatedTileList())
    {
      animatedTile.setOffset(Vector2(-this->_offset.x, 0));
    }

    for (Rectangle &collisionRectangle : this->_level->getCollisionRects())
    {
      collisionRectangle.setOffset(Vector2(-this->_offset.x, 0));
    }
    for (Rectangle &door : this->_level->getDoorsList())
    {
      door.setOffset(Vector2(-this->_offset.x, 0));
    }
    for (Rectangle &levelPassage : this->_level->getLevelPassagesList())
    {
      levelPassage.setOffset(Vector2(-this->_offset.x, 0));
    }

    // for (Enemy *enemy : this->_level->getEnemiesList())
    // {
    //   enemy.setOffset(Vector2(-cameraX, 0));
    // }
  }
}

void Camera::handleScrollOffset(int playerX)
{
  if (this->_player->getXVelocity() != 0 && !this->_player->shouldMoveCamera())
  {
    float playerDistanceRelativeToCenter = playerX - globals::SCREEN_WIDTH / 2;
    playerDistanceRelativeToCenter = std::max(0.0f, std::min(playerDistanceRelativeToCenter, (this->_level->getSize().x * this->_level->getTileSize().x * globals::SPRITE_SCALE) - globals::SCREEN_WIDTH));
    this->_offset.x = playerDistanceRelativeToCenter;
  }
  else if (this->_player->shouldMoveCamera())
  {
    this->_offset.x += 0.2;
  }
}
