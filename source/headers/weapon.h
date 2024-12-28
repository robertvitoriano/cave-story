#ifndef WEAPON_H
#define WEAPON_H

#include "animatedsprite.h"
#include "globals.h"

class Graphics;

class Weapon : public AnimatedSprite
{
public:
  Weapon();
  Weapon(Graphics &graphics, std::string filePath, int sourceX, int sourceY,
         int width, int height, Vector2 spawnPoint, int timeToUpdate);

  virtual void update(int elapsedTime);
  virtual void draw(Graphics &graphics);

protected:
  Direction _direction;
};

#endif // WEAPON_H
