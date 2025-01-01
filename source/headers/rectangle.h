#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "globals.h"
#include "graphics.h"
class Rectangle
{
public:
	Rectangle() {}

	Rectangle(int x, int y, int width, int height) : _x(x),
																									 _y(y),
																									 _width(width),
																									 _height(height)
	{
	}

	const int getCenterX() const { return this->_x + this->_width / 2; }
	const int getCenterY() const { return this->_y + this->_height / 2; }

	const int getLeft() const { return this->_x; }
	const int getRight() const { return this->_x + this->_width; }
	const int getTop() const { return this->_y; }
	const int getBottom() const { return this->_y + this->_height; }

	const int getWidth() const { return this->_width; }
	const int getHeight() const { return this->_height; }
	void setPosition(Vector2 position)
	{
		this->_x = position.x;
		this->_y = position.y;
	}
	Vector2 getPosition() const
	{
		return {this->_x, this->_y};
	}

	const int getSide(const sides::Side side) const
	{
		return side == sides::LEFT ? this->getLeft() : side == sides::RIGHT ? this->getRight()
																							 : side == sides::TOP			? this->getTop()
																							 : side == sides::BOTTOM	? this->getBottom()
																																				: sides::NONE;
	}

	const bool collidesWith(const Rectangle &other) const
	{
		return this->getRight() >= other.getLeft() && // COLLISION BY THE LEFT
					 this->getLeft() <= other.getRight() && // COLLISION BY THE RIGHT
					 this->getTop() <= other.getBottom() && // COLLISION BELLOW
					 this->getBottom() >= other.getTop();		// TOP COLLISION
	}

	const bool isValidRectangle() const
	{
		return (this->_x >= 0 && this->_y >= 0 && this->_width >= 0 && this->_height >= 0);
	}

	const inline Rectangle getRect() const { return *this; }

	void draw(Graphics &graphics)
	{
		SDL_Rect rect = {this->_x, this->_y, this->_width, this->_height};
		SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 255, 255, 255);
		SDL_RenderDrawRect(graphics.getRenderer(), &rect);
		SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 0, 0, 0);
	}

private:
	int _x, _y, _width, _height;
};

#endif
