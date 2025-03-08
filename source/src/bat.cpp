#include <bat.h>

Bat::Bat() {}

Bat::Bat(Graphics &graphics, Vector2 spawnPoint)
    : Enemy(graphics, "content/sprites/NpcCemet-no-bg.png", 32, 32, 16, 16, spawnPoint, 140),
      _startingX(spawnPoint.x), _startingY(spawnPoint.y), _shouldMoveUp(false), _wasHit(false)
{
  this->setupAnimations();
  this->playAnimation("FlyLeft");
}

void Bat::update(int elapsedTime, Player &player)
{
  double flightSpeed = 0.03;
  int flightLimit = 30;

  this->_direction = player.getX() > this->_x ? RIGHT : LEFT;
  this->playAnimation(this->_direction == RIGHT ? "FlyRight" : "FlyLeft");

  this->_y += this->_shouldMoveUp ? -flightSpeed : flightSpeed;

  if (this->_y > (this->_startingY + flightLimit) || this->_y < this->_startingY - flightLimit)
  {
    this->_shouldMoveUp = !this->_shouldMoveUp;
  }

  Enemy::update(elapsedTime, player);
}

void Bat::draw(Graphics &graphics, Vector2 offset)
{
  Enemy::draw(graphics, offset);
}

void Bat::animationDone(std::string currentAnimation)
{
  // Handle the completion of an animation if needed
}

void Bat::setupAnimations()
{
  this->addAnimation(3, 2, 32, "FlyLeft", 16, 16, Vector2(0, 0), "row");
  this->addAnimation(3, 2, 48, "FlyRight", 16, 16, Vector2(0, 0), "row");
}

void Bat::touchPlayer(Player *player)
{
  player->gainHealth(-1);
}

bool Bat::shouldBeDestroyed()
{
  return this->_wasHit;
}
