#ifndef CAMERA_H
#define CAMERA_H

#include "globals.h"
#include <SDL2/SDL.h>
#include "level.h"
#include "door.h"
#include "LevelPassage.h"
#include "rectangle.h"
#include "player.h"
#include "enemy.h"
#include "rectangle.h"

class Camera : public Rectangle
{
public:
  Camera(const Camera &) = delete;
  Camera &operator=(const Camera &) = delete;

  static Camera &getInstance();

  Vector2 getCenter();
  void setCenter(Vector2 position);
  int getRightLimit();
  void drawDebug(SDL_Renderer *renderer);
  void update();
  void follow(Player *player, Level *level);

private:
  Camera();

  int _speed;
  int _rightLimit;
  Vector2 _center;
  Level *_level;
  Player *_player;
  float _lastOffset;
  float _dx,
      _dy;
};

#endif
