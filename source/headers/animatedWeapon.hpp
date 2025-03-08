#ifndef AnimatedWeapon_H
#define AnimatedWeapon_H

#include <animatedsprite.h>
#include <globals.h>

class Graphics;

class AnimatedWeapon : public AnimatedSprite
{
public:
  AnimatedWeapon();
  AnimatedWeapon(Graphics &graphics, std::string filePath, int sourceX, int sourceY,
                 int width, int height, Vector2 spawnPoint, int timeToUpdate);

  virtual void update(int elapsedTime);
  virtual void draw(Graphics &graphics, int x, int y);

protected:
  Direction _direction;
};

#endif // AnimatedWeapon_H
