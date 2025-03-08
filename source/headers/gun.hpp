#ifndef GUN_HPP
#define GUN_HPP
#include <weapon.hpp>
class Gun : public Weapon
{
public:
  Gun();
  Gun(Graphics &graphics, Vector2 spawnPoint);
  void update(int elapsedTime);
  void draw(Graphics &graphics, int x, int y);
};

#endif
