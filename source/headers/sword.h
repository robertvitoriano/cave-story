#ifndef SWORD_H
#define SWORD_H

#include "weapon.h"
#include <iostream>
class Sword : public Weapon
{
public:
  Sword();
  Sword(Graphics &graphics, Vector2 spawnPoint);

  void update(int elapsedTime) override;
  void draw(Graphics &graphics) override;
  void animationDone(std::string currentAnimation);
  void setupAnimations();

private:
  float _startingX;
  float _startingY;
};

#endif // SWORD_H
