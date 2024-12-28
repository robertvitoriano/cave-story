#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "level.h"
#include "hud.h"
#include "graphics.h"
#include "MusicPlayer.h"
#include "LevelPassage.h"
#include "globals.h"
class Game
{
public:
	Game();
	~Game();

private:
	void gameLoop();
	void draw(Graphics &graphics);
	void update(float elapsedTime, Graphics &graphics);

	Player _player;

	Level _level;

	HUD _hud;

	bool gameIsLost;
};

#endif
