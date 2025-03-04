#include <gun.hpp>

Gun::Gun()
{
}

Gun::Gun(Graphics &graphics, Vector2 spawnPoint) : Weapon(graphics, "content/sprites/Arms.png", 0, 0, 16, 16, spawnPoint, 140)
{
}

void Gun::draw(Graphics &graphics, int x, int y)
{
}

void Gun::update(int elapsedTime)
{
}
