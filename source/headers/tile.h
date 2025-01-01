#ifndef TILE_H
#define TILE_H

#include "globals.h";

struct SDL_Texture;
class Graphics;

class Tile
{
public:
	Tile();
	Tile(SDL_Texture *tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position);
	void update(int elapsedTime);
	void draw(Graphics &graphics, Vector2 offset);
	Vector2 getPosition();
	Vector2 getSize();
	void setPosition(Vector2 position);
	void setOffset(Vector2 offset);

protected:
	SDL_Texture *_tileset;
	Vector2 _size;
	Vector2 _tilesetPosition;
	Vector2 _position;
	Vector2 _offset;
};

#endif
