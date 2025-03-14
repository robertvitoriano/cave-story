#include <animatedWeapon.hpp>

AnimatedWeapon::AnimatedWeapon() {}

AnimatedWeapon::AnimatedWeapon(Graphics &graphics, std::string filePath, int sourceX, int sourceY,
                               int width, int height, Vector2 spawnPoint, int timeToUpdate)
    : AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height,
                     spawnPoint.x, spawnPoint.y, timeToUpdate),
      _direction(LEFT)
{
}

void AnimatedWeapon::update(int elapsedTime)
{
  AnimatedSprite::update(elapsedTime);
}

void AnimatedWeapon::draw(Graphics &graphics, int x, int y)
{

  AnimatedSprite::draw(graphics, x, y);
}
