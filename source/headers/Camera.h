#ifndef CAMERA_H
#define CAMERA_H

#include <globals.h>
#include <SDL2/SDL.h>
#include <level.h>
#include <door.h>
#include <LevelPassage.h>
#include <rectangle.h>
#include <player.h>
#include <enemy.h>
#include <rectangle.h>
#include <slope.h>

class Camera : public Rectangle
{
public:
  Camera(const Camera &) = delete;
  Camera &operator=(const Camera &) = delete;

  static Camera &getInstance();

  Vector2 getCenter();
  void setCenter(Vector2 position);
  int getRightLimit();
  void drawDebug(Graphics &graphics);
  void update(float elapsedTime);
  void follow(Player *player, Level *level);
  void stopMoving();
  bool reachedMaxXScroll();
  float getMaxXScroll();
  void moveLeft();
  void moveRight();

private:
  Camera();
  void handleScrollOffset(float elapsedTime);

  void move(float newXOffset);

  int _rightLimit;
  float _maxXScroll;
  Vector2 _center;
  Level *_level;
  Player *_player;
  float _dx,
      _dy;

  Uint32 _moveTimer;
  Uint32 _moveSpeedDelay;
};

#endif
