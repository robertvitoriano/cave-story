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
Camera::Camera() : Rectangle(0, 0, globals ::SCREEN_WIDTH,
                             globals::SCREEN_HEIGHT),
                   _dx(camera_constants::SPEED), _center({globals::SCREEN_WIDTH / 2, globals::SCREEN_HEIGHT / 2}), _rightLimit(globals::SCREEN_WIDTH - 100), _moveTimer(0), _moveSpeedDelay(5), _maxXScroll(0.0f), _moveCamera(false)
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

void Camera::drawDebug(Graphics &graphics)
{
  SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);

  SDL_Rect centerRect = {
      static_cast<int>(this->_center.x - 2),
      static_cast<int>(this->_center.y - 2),
      4,
      4};
  SDL_RenderFillRect(graphics.getRenderer(), &centerRect);

  SDL_RenderDrawLine(graphics.getRenderer(),
                     this->_rightLimit,
                     0,
                     this->_rightLimit,
                     this->getHeight());

  SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 255, 255, SDL_ALPHA_OPAQUE);

  graphics.drawText("Scroll X: " + std::to_string(this->_offset.x), {255, 255, 255}, {globals::SCREEN_WIDTH / 2, globals::SCREEN_HEIGHT - 20});
}

void Camera::follow(Player *player, Level *level)
{
  this->_player = player;
  this->_level = level;
  this->_offset.x = 0;
}
void Camera::update(float elapsedTime)
{
  if (!_player)
  {
    return;
  }
  float playerX = this->_player->getX();

  if (this->_level->isLevelWiderThanScreen())
  {
    this->handleScrollOffset(playerX, elapsedTime);

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
    for (Slope &slope : this->_level->getSlopesList())
    {
      slope.setOffset(Vector2(-this->_offset.x, 0));
    }

    // for (Enemy *enemy : this->_level->getEnemiesList())
    // {
    //   enemy.setOffset(Vector2(-cameraX, 0));
    // }
  }
}

void Camera::handleScrollOffset(int playerX, float elapsedTime)
{
  float levelWidth = this->_level->getSize().x * this->_level->getTileSize().x * globals::SPRITE_SCALE;
  this->_maxXScroll = levelWidth - globals::SCREEN_WIDTH;

  int cameraXMiddle = globals::SCREEN_WIDTH / 2;
  bool playerIsMoving = this->_player->getXVelocity() != 0;
  float newXOffset = std::min(this->_dx * elapsedTime, this->_maxXScroll);
  if ((this->reachedMaxXScroll() && this->_player->getFacing() == RIGHT) || this->_offset.x + newXOffset < 0)
  {
    this->_player->enableVelocity();
    this->stopMoving();
    return;
  }
  this->_player->disableVelocity();
  this->_offset.x += newXOffset;
}

void Camera::moveLeft()
{
  this->_dx = camera_constants::SPEED;
}
void Camera::moveRight()
{
  this->_dx = -camera_constants::SPEED;
}
void Camera::stopMoving()
{
  this->_dx = 0;
  this->_moveCamera = false;
}

bool Camera::reachedMaxXScroll()
{
  return static_cast<float>(this->_offset.x) >= this->_maxXScroll;
}

void Camera::startMoving()
{
  this->_moveCamera = true;
}
bool Camera::cameraIsMoving()
{
  return this->_moveCamera;
}
float Camera::getMaxXScroll()
{
  return this->_maxXScroll;
}
