#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>

#include <string>

#include <rectangle.h>
#include <globals.h>

class Graphics;

/* Sprite class
 * Holds all information for individual sprites
 */

class Sprite
{
public:
	Sprite();
	Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
				 float posX, float posY);
	virtual ~Sprite();
	virtual void update();
	void draw(Graphics &graphics, int x, int y);

	Rectangle &getBoundingBox();
	sides::Side getCollisionSide(Rectangle &other);

	const inline float getX() const { return this->_x; }
	const inline float getY() const { return this->_y; }
	void setX(int x) { this->_x = x; }
	void setY(int y) { this->_y = y; }
	void setPosition(Vector2 position);
	Vector2 getPosition() { return {this->_x, this->_y}; }
	void setSourceRectX(int value);
	void setSourceRectY(int value);
	void setSourceRectW(int value);
	void setSourceRectH(int value);

protected:
	SDL_Rect _sourceRect;
	SDL_Texture *_spriteSheet;

	Rectangle _boundingBox;

	float _x, _y;

private:
};

#endif
