#ifndef BAT_H
#define BAT_H

#include <enemy.h>
#include <player.h>
#include <iostream>
class Bat : public Enemy
{
public:
  Bat();
  Bat(Graphics &graphics, Vector2 spawnPoint);

  void update(int elapsedTime, Player &player) override;
  void draw(Graphics &graphics, Vector2 offset) override;
  void animationDone(std::string currentAnimation);
  void setupAnimations();
  void touchPlayer(Player *player) override;
  bool shouldBeDestroyed() override;

private:
  bool _shouldMoveUp;
  bool _wasHit;
  float _startingX;
  float _startingY;
};

#endif // BAT_H
