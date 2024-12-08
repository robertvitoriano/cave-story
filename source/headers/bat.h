#ifndef BAT_H
#define BAT_H

#include "enemy.h"

class Bat : public Enemy
{
public:
  Bat();
  Bat(Graphics &graphics, Vector2 spawnPoint);

  void update(int elapsedTime, Player &player) override;
  void draw(Graphics &graphics) override;
  void animationDone(std::string currentAnimation) override;
  void setupAnimations() override;
  void touchPlayer(Player *player) override;

private:
  bool _shouldMoveUp;
  float _startingX;
  float _startingY;
};

#endif
