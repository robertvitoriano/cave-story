#ifndef SLOPE_H
#define SLOPE_H

#include <globals.h>
#include <cmath>
#include <SDL2/SDL.h>

class Slope
{
public:
	Slope() {}
	Slope(Vector2 p1, Vector2 p2)
			: _p1(p1), _p2(p2), _offset(Vector2(0, 0))
	{
		if (this->_p2.x - this->_p1.x != 0)
		{
			this->_slope = (fabs(this->_p2.y) - fabs(this->_p1.y)) /
										 (fabs(this->_p2.x) - fabs(this->_p1.x));
		}
	}

	float getSlope()
	{
		return this->_slope;
	}
	void setFirstPointPosition(Vector2 position)
	{
		this->_p1.x = position.x;
		this->_p1.y = position.y;
	}

	void setSecondPointPosition(Vector2 position)
	{
		this->_p2.x = position.x;
		this->_p2.y = position.y;
	}

	bool collidesWith(Rectangle &other)
	{
		return (other.getRight() >= this->getP2X() &&
						other.getLeft() <= this->getP1X() &&
						other.getTop() <= this->getP2Y() &&
						other.getBottom() >= this->getP1Y()) ||
					 (other.getRight() >= this->getP1X() &&
						other.getLeft() <= this->getP2X() &&
						other.getTop() <= this->getP1Y() &&
						other.getBottom() >= this->getP2Y()) ||
					 (other.getLeft() <= this->getP1X() &&
						other.getRight() >= this->getP2X() &&
						other.getTop() <= this->getP1Y() &&
						other.getBottom() >= this->getP2Y()) ||
					 (other.getLeft() <= this->getP2X() &&
						other.getRight() >= this->getP1X() &&
						other.getTop() <= this->getP2Y() &&
						other.getBottom() >= this->getP1Y());
	}

	Vector2 getP1() { return this->_p1; }
	Vector2 getP2() { return this->_p2; }
	void setOffset(Vector2 offset)
	{
		this->_offset = offset;
	}
	float getP1X()
	{
		return this->_p1.x + this->_offset.x;
	}
	float getP1Y()
	{
		return this->_p1.y + this->_offset.y;
	}
	float getP2X()
	{
		return this->_p2.x + this->_offset.x;
	}
	float getP2Y()
	{
		return this->_p2.y + this->_offset.y;
	}

	void draw(Graphics &graphics)
	{
		SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 255, 255, 255);

		SDL_RenderDrawLine(graphics.getRenderer(), static_cast<int>(getP1X()), static_cast<int>(getP1Y()),
											 static_cast<int>(getP2X()), static_cast<int>(getP2Y()));
	}

private:
	Vector2 _p1, _p2;
	float _slope;
	Vector2 _offset;
};

#endif
