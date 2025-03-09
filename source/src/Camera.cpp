#include <Camera.h>

namespace camera_constants
{
  const float SPEED = 0.2f;
}

Camera &Camera::getInstance()
{
  static Camera instance;
  return instance;
}
Camera::Camera()
    : Rectangle(0, 0, globals ::SCREEN_WIDTH, globals::SCREEN_HEIGHT),
      _dx(camera_constants::SPEED),
      _center({globals::SCREEN_WIDTH / 2, globals::SCREEN_HEIGHT / 2}),
      _rightLimit(globals::SCREEN_WIDTH - 100), _moveTimer(0),
      _moveSpeedDelay(5), _maxXScroll(0.0f)
{
  this->_offset = {0, 0};
}

Vector2 Camera::getCenter() { return this->_center; }

void Camera::setCenter(Vector2 position) { this->_center = position; }

int Camera::getRightLimit() { return this->_rightLimit; }

void Camera::drawDebug(Graphics &graphics)
{
  SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);

  SDL_Rect centerRect = {static_cast<int>(this->_center.x - 2),
                         static_cast<int>(this->_center.y - 2), 4, 4};

  SDL_RenderFillRect(graphics.getRenderer(), &centerRect);

  SDL_RenderDrawLine(graphics.getRenderer(), this->_rightLimit, 0,
                     this->_rightLimit, this->getHeight());

  SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 255, 255,
                         SDL_ALPHA_OPAQUE);

  graphics.drawText("Scroll X: " + std::to_string(this->_offset.x),
                    {255, 255, 255},
                    {globals::SCREEN_WIDTH / 2, globals::SCREEN_HEIGHT - 20});
}

void Camera::follow(Player *player, Level *level)
{
  this->_player = player;
  this->_player->enableVelocity();
  this->_level = level;
  this->_offset.x = 0;
  this->_offset.y = 0;
}
void Camera::update(float elapsedTime)
{
  if (!_player)
  {
    return;
  }
  Vector2 offsetVector = Vector2(-this->_offset.x, -this->_offset.y);

  if (this->_level->isLevelWiderThanScreen())
  {
    this->handleScrollOffset(elapsedTime);

    for (Tile &tile : this->_level->getTileList())
    {
      tile.setOffset(offsetVector);
    }

    for (AnimatedTile &animatedTile : this->_level->getAnimatedTileList())
    {
      animatedTile.setOffset(offsetVector);
    }

    for (Rectangle &collisionRectangle : this->_level->getCollisionRects())
    {
      collisionRectangle.setOffset(offsetVector);
    }
    for (Rectangle &door : this->_level->getDoorsList())
    {
      door.setOffset(offsetVector);
    }
    for (Rectangle &levelPassage : this->_level->getLevelPassagesList())
    {
      levelPassage.setOffset(offsetVector);
    }
    for (Slope &slope : this->_level->getSlopesList())
    {
      slope.setOffset(offsetVector);
    }

    // for (Enemy *enemy : this->_level->getEnemiesList())
    // {
    //   enemy.setOffset(Vector2(-cameraX, 0));
    // }
  }
}

void Camera::handleScrollOffset(float elapsedTime)
{
  float levelWidth = this->_level->getSize().x * this->_level->getTileSize().x * globals::SPRITE_SCALE;

  this->_maxXScroll = levelWidth - globals::SCREEN_WIDTH;

  if ((this->reachedMaxXScroll() && this->_player->getFacing() == RIGHT) || this->_offset.x <= 0 && this->_player->getFacing() == LEFT)
  {
    this->_player->enableVelocity();
    this->stopMoving();
    return;
  }

  this->_player->disableVelocity();

  float newXOffset = std::min(this->_dx * elapsedTime, this->_maxXScroll);

  Rectangle collidingRect = this->_player->getCollidingRect();
  sides::Side collisionSide = this->_player->getCollisionSide(collidingRect);

  bool collidedHorizontally = collisionSide == sides::RIGHT || collisionSide == sides::LEFT;

  if (!collidedHorizontally)
  {
    this->move(newXOffset);
  }
}

void Camera::move(float newXOffset) { this->_offset.x += newXOffset; }

void Camera::moveLeft() { this->_dx = camera_constants::SPEED; }

void Camera::moveRight() { this->_dx = -camera_constants::SPEED; }

void Camera::stopMoving()
{
  this->_dx = 0;
}

bool Camera::reachedMaxXScroll()
{
  return static_cast<float>(this->_offset.x) >= this->_maxXScroll;
}

float Camera::getMaxXScroll() { return this->_maxXScroll; }
