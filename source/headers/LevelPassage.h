#ifndef LEVEL_PASSAGE_H
#define LEVEL_PASSAGE_H

#include "globals.h"
#include "rectangle.h"
#include <string>
#include <iostream>
class LevelPassage : public Rectangle
{
public:
  LevelPassage() {}
  LevelPassage(Rectangle r, std::string destination, Vector2 spawnPosition, bool enableGravity) : Rectangle(r.getLeft() * globals::SPRITE_SCALE, r.getTop() * globals::SPRITE_SCALE,
                                                                                                            r.getWidth() * globals::SPRITE_SCALE, r.getHeight() * globals::SPRITE_SCALE),
                                                                                                  _destination(destination), _spawnPosition(spawnPosition),
                                                                                                  _enableGravity(enableGravity)
  {
  }

  Rectangle getRectangle() { return this->getRect(); }
  std::string getDestination() { return this->_destination; }
  Vector2 getSpawnPosition() { return this->_spawnPosition; }
  bool shouldEnableGravity() { return this->_enableGravity; }

private:
  std::string _destination;
  Vector2 _spawnPosition;
  bool _enableGravity;
};

#endif
