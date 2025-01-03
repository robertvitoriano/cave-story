#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <globals.h>
#include <graphics.h>
class Rectangle
{
public:
	Rectangle() {}

	Rectangle(int x, int y, int width, int height) : _x(x),
																									 _y(y),
																									 _width(width),
																									 _height(height),
																									 _offset(Vector2(0, 0))
	{
	}

	int getCenterX() { return this->getX() + this->_width / 2; }
	int getCenterY() { return this->getY() + this->_height / 2; }

	int getLeft() { return this->getX(); }
	int getRight() { return this->getX() + this->_width; }
	int getTop() { return this->getY(); }
	int getBottom() { return this->getY() + this->_height; }

	int getWidth() { return this->_width; }
	int getHeight() { return this->_height; }
	void setPosition(Vector2 position)
	{
		this->_x = position.x;
		this->_y = position.y;
	}
	Vector2 getPosition()
	{
		return {this->_x, this->_y};
	}

	int getSide(sides::Side side)
	{
		return side == sides::LEFT ? this->getLeft() : side == sides::RIGHT ? this->getRight()
																							 : side == sides::TOP			? this->getTop()
																							 : side == sides::BOTTOM	? this->getBottom()
																																				: sides::NONE;
	}

	bool collidesWith(Rectangle &other)
	{
		return this->getRight() >= other.getLeft() && // COLLISION BY THE LEFT
					 this->getLeft() <= other.getRight() && // COLLISION BY THE RIGHT
					 this->getTop() <= other.getBottom() && // COLLISION BELLOW
					 this->getBottom() >= other.getTop();		// TOP COLLISION
	}

	bool isValidRectangle()
	{
		return (this->_x >= 0 && this->_y >= 0 && this->_width >= 0 && this->_height >= 0);
	}

	inline Rectangle getRect() { return *this; }

	void draw(Graphics &graphics)
	{
		SDL_Rect rect = {this->getX(), this->getY(), this->_width, this->_height};
		SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 255, 255, 255);
		SDL_RenderDrawRect(graphics.getRenderer(), &rect);
		SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 0, 0, 0);
	}
	void setOffset(Vector2 offset)
	{
		this->_offset = offset;
	}

	int getX()
	{
		return this->_x + this->_offset.x;
	}
	int getY()
	{
		return this->_y + this->_offset.y;
	}

protected:
	Vector2 _offset;
	float _x, _y;

	int _width,
			_height;
};

#endif
