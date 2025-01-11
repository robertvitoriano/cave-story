#ifndef GAME_H
#define GAME_H

#include <player.h>
#include <level.h>
#include <hud.h>
#include <graphics.h>
#include <MusicPlayer.h>
#include <LevelPassage.h>
#include <globals.h>
#include <input.h>
#include <SDL2/SDL.h>
#include <sword.h>
#include <GravityChange.h>
#include <Camera.h>
#include <Menu.h>
#include <MenuItem.h>
#include <MenuManager.h>

class Game
{
public:
	Game();
	~Game();

private:
	void gameLoop();
	void draw(Graphics &graphics);
	void update(float elapsedTime, Graphics &graphics, Input &input);
	void handleInput(Input &input, float elapsedTime);
	void handleScrolling();
	void toggleDebug();
	Player _player;

	Level _level;

	HUD _hud;
	bool gameIsLost;
	bool _displayDebug;
	SDL_Joystick *_joystick;
	MenuManager _menuManager;
	std::shared_ptr<Menu> _mainMenu;
};

#endif
