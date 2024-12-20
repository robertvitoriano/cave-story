#ifndef DOOR_H
#define DOOR_H

#include "rectangle.h";
#include "globals.h"

#include <string>

class Door : public Rectangle
{
public:
	Door() {}
	Door(Rectangle r, std::string destination, Vector2 spawnPosition) : Rectangle(r.getLeft() * globals::SPRITE_SCALE, r.getTop() * globals::SPRITE_SCALE,
																																								r.getWidth() * globals::SPRITE_SCALE, r.getHeight() * globals::SPRITE_SCALE),
																																			_destination(destination), _spawnPosition(spawnPosition)
	{
	}
	const Vector2 getSpawnPosition() const { return this->_spawnPosition; }

	const inline Rectangle getRectangle() const { return this->getRect(); }
	const inline std::string getDestination() const { return this->_destination; }

private:
	std::string _destination;
	Vector2 _spawnPosition;
};

#endif
