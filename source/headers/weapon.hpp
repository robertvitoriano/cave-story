#ifndef WEAPON_H
#define WEAPON_H

#include <sprite.h>
#include <globals.h>

class Weapon : public Sprite
{
public:
  Weapon();
  Weapon(Graphics &graphics, std::string filePath, int sourceX, int sourceY,
         int width, int height, Vector2 spawnPoint, int timeToUpdate);

  virtual void update(int elapsedTime);
  virtual void draw(Graphics &graphics, int x, int y);

protected:
  Direction _direction;
};

#endif // WEAPON_H
