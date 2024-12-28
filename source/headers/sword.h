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
  void draw(Graphics &graphics, int x, int y, Direction direction);
  void animationDone(std::string currentAnimation);
  void setupAnimations();
  void performAttack();
  void stopAttack();

private:
  float _startingX;
  float _startingY;
  Direction _direction;
  bool _isAttacking;
};

#endif // SWORD_H
