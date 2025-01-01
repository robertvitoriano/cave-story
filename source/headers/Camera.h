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
class Camera
{
public:
  Camera(const Camera &) = delete;
  Camera &operator=(const Camera &) = delete;

  static Camera &getInstance();

  int getWidth();
  int getHeight();
  Vector2 getCenter();
  void setCenter(Vector2 position);
  int getRightLimit();
  void drawDebug(SDL_Renderer *renderer);
  void update();
  void follow(Player *player, Level *level);

private:
  Camera();

  int _speed;
  int _width;
  int _height;
  int _rightLimit;
  Vector2 _center;
  Level *_level;
  Player *_player;
};

#endif
