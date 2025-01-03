#ifndef GRAVITY_CHANGE_H
#define GRAVITY_CHANGE_H

#include <globals.h>
#include <rectangle.h>
#include <string>
#include <iostream>
class GravityChange : public Rectangle
{
public:
  GravityChange() {}
  GravityChange(Rectangle r, bool enableGravity) : Rectangle(r.getLeft() * globals::SPRITE_SCALE, r.getTop() * globals::SPRITE_SCALE,
                                                             r.getWidth() * globals::SPRITE_SCALE, r.getHeight() * globals::SPRITE_SCALE),
                                                   _enableGravity(enableGravity)
  {
  }

  Rectangle getRectangle() { return this->getRect(); }
  bool shouldEnableGravity() { return this->_enableGravity; }

private:
  bool _enableGravity;
};

#endif
