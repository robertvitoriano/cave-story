#include <weapon.hpp>

Weapon::Weapon() {}

Weapon::Weapon(Graphics &graphics, std::string filePath, int sourceX, int sourceY,
               int width, int height, Vector2 spawnPoint, int timeToUpdate)
    : Sprite(graphics, filePath, sourceX, sourceY, width, height, 100, 100),
      _direction(LEFT)
{
}

void Weapon::update(int elapsedTime)
{
  std::cout << "Update weapon" << std::endl;
}

void Weapon::draw(Graphics &graphics, int x, int y)
{

  Sprite::draw(graphics, x, y);
}
