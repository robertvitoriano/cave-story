#include "sword.h"

Sword::Sword() {}

Sword::Sword(Graphics &graphics, Vector2 spawnPoint)
    : Weapon(graphics, "content/sprites/sword.png", 0, 32, 16, 16, spawnPoint, 140),
      _startingX(spawnPoint.x), _startingY(spawnPoint.y)
{
  this->setupAnimations();
  this->playAnimation("sword-idle");
}

void Sword::update(int elapsedTime)
{

  Weapon::update(elapsedTime);
}

void Sword::draw(Graphics &graphics)
{
  Weapon::draw(graphics);
}

void Sword::animationDone(std::string currentAnimation)
{
  // Handle the completion of an animation if needed
}

void Sword::setupAnimations()
{
  this->addAnimation(1, 0, 0, "sword-idle", 16, 16, Vector2(0, 0), "column");

  this->addAnimation(2, 0, 0, "sword-attack", 16, 16, Vector2(0, 0), "column");
}

void Sword::performAttack()
{
  this->playAnimation("sword-attack");
}
