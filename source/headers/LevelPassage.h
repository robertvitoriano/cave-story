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
  LevelPassage(Rectangle r, std::string destination) : Rectangle(r.getLeft() * globals::SPRITE_SCALE, r.getTop() * globals::SPRITE_SCALE,
                                                                 r.getWidth() * globals::SPRITE_SCALE, r.getHeight() * globals::SPRITE_SCALE),
                                                       _destination(destination)
  {
  }

  const inline Rectangle getRectangle() const { return this->getRect(); }
  const inline std::string getDestination() const { return this->_destination; }

private:
  std::string _destination;
};

#endif
