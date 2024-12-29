#ifndef GLOBALS_H
#define GLOBALS_H

namespace globals
{
	const int SCREEN_ORIGINAL_WIDTH = 320;
	const int SCREEN_ORIGINAL_HEIGHT = 256;
	const float SPRITE_SCALE = 2.0f;
	const int SCREEN_WIDTH = SCREEN_ORIGINAL_WIDTH * SPRITE_SCALE;
	const int SCREEN_HEIGHT = SCREEN_ORIGINAL_HEIGHT * SPRITE_SCALE;

}

namespace sides
{
	enum Side
	{
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		NONE
	};

	inline Side getOppositeSide(Side side)
	{
		return side == TOP ? BOTTOM : side == BOTTOM ? TOP
															: side == LEFT		 ? RIGHT
															: side == RIGHT		 ? LEFT
																								 : NONE;
	}
}

enum Direction
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

struct Vector2
{
	int x, y;
	Vector2() : x(0), y(0)
	{
	}
	Vector2(int x, int y) : x(x), y(y)
	{
	}
	Vector2 zero()
	{
		return Vector2(0, 0);
	}
};

#endif
