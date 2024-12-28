#include "sword.h"

Sword::Sword() {}

Sword::Sword(Graphics &graphics, Vector2 spawnPoint)
    : Weapon(graphics, "content/sprites/sword.png", 0, 0, 16, 16, spawnPoint, 140),
      _startingX(spawnPoint.x), _startingY(spawnPoint.y), _direction(RIGHT)
{
  this->setupAnimations();
  this->playAnimation(_direction == RIGHT ? "sword-idle" : "sword-idle-left");
}

void Sword::update(int elapsedTime)
{
  if (!this->_isAttacking)
  {
    this->playAnimation(_direction == RIGHT ? "sword-idle" : "sword-idle-left");
  }
  Weapon::update(elapsedTime);
}

void Sword::draw(Graphics &graphics, int x, int y, Direction direction)
{
  this->_direction = direction;
  if (this->_direction == RIGHT)
  {
    Weapon::draw(graphics, x + 15, y);
  }
  if (this->_direction == LEFT)
  {
    Weapon::draw(graphics, x - 15, y);
  }
}

void Sword::animationDone(std::string currentAnimation)
{
  // Handle the completion of an animation if needed
}

void Sword::setupAnimations()
{
  this->addAnimation(1, 0, 0, "sword-idle", 16, 16, Vector2(0, 0), "column");
  this->addAnimation(2, 0, 0, "sword-attack", 16, 16, Vector2(0, 0), "column");
  this->addAnimation(1, 1, 0, "sword-idle-left", 16, 16, Vector2(0, 0), "column");
  this->addAnimation(2, 1, 0, "sword-attack-left", 16, 16, Vector2(0, 0), "column");
}

void Sword::performAttack()
{
  this->_isAttacking = true;
  this->playAnimation(_direction == RIGHT ? "sword-attack" : "sword-attack-left");
}
void Sword::stopAttack()
{

  this->playAnimation(_direction == RIGHT ? "sword-idle" : "sword-idle-left");
  this->_isAttacking = false;
}
