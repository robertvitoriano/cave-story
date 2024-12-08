#include "bat.h"

Bat::Bat() : Enemy() {}

Bat::Bat(Graphics &graphics, Vector2 spawnPoint)
    : Enemy(graphics, "content/sprites/NpcCemet.png", 32, 32, 16, 16, spawnPoint, 140),
      _startingX(spawnPoint.x),
      _startingY(spawnPoint.y),
      _shouldMoveUp(false)
{
  this->setupAnimations();
  this->playAnimation("FlyLeft");
}

void Bat::update(int elapsedTime, Player &player)
{
  this->_direction = player.getX() > this->_x ? RIGHT : LEFT;
  this->playAnimation(this->_direction == RIGHT ? "FlyRight" : "FlyLeft");

  this->_y += this->_shouldMoveUp ? -0.4f : 0.4f;
  if (this->_y > (_startingY + 30) || this->_y < (_startingY - 30))
  {
    this->_shouldMoveUp = !_shouldMoveUp;
  }

  Enemy::update(elapsedTime, player);
}

void Bat::draw(Graphics &graphics)
{
  Enemy::draw(graphics);
}

void Bat::animationDone(std::string currentAnimation)
{
  (void)currentAnimation;
}

void Bat::setupAnimations()
{
  this->addAnimation(3, 2, 32, "FlyLeft", 16, 16, Vector2(0, 0));
  this->addAnimation(3, 2, 48, "FlyRight", 16, 16, Vector2(0, 0));
}

void Bat::touchPlayer(Player *player)
{
  player->gainHealth(-1);
}
