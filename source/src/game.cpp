#include <SDL2/SDL.h>

#include "game.h"
#include "graphics.h"
#include "input.h"
namespace
{
	const int FPS = 50;
	const int MAX_FRAME_TIME = 1000 / FPS;
}

Game::Game()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	MusicPlayer &musicPlayer = MusicPlayer::getInstance();
	musicPlayer.playSound("content/sounds/background.mp3", -1);
	this->gameLoop();
}

Game::~Game()
{
}

void Game::gameLoop()
{
	Graphics graphics;
	Input input;
	SDL_Event event;

	this->_level = Level("Map_1", graphics);
	this->_player = Player(graphics, this->_level.getPlayerSpawnPoint());
	this->_hud = HUD(graphics, this->_player);

	int LAST_UPDATE_TIME = SDL_GetTicks();
	// Start the game loop
	while (true)
	{
		input.beginNewFrame();

		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.repeat == 0)
				{
					input.keyDownEvent(event);
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				input.keyUpEvent(event);
			}
			else if (event.type == SDL_QUIT)
			{
				return;
			}
		}
		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true)
		{
			return;
		}
		else if (input.isKeyHeld(SDL_SCANCODE_LEFT) == true)
		{
			this->_player.moveLeft();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_RIGHT) == true)
		{
			this->_player.moveRight();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_UP) == true && !this->_player.isGravityEnabled())
		{
			this->_player.moveUp();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_DOWN) == true && !this->_player.isGravityEnabled())
		{
			this->_player.moveDown();
		}

		if (input.isKeyHeld(SDL_SCANCODE_UP) == true && this->_player.isGravityEnabled())
		{
			this->_player.lookUp();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_DOWN) == true && this->_player.isGravityEnabled())
		{
			this->_player.lookDown();
		}

		if (input.wasKeyReleased(SDL_SCANCODE_UP) == true)
		{
			this->_player.stopLookingUp();
		}
		if (input.wasKeyReleased(SDL_SCANCODE_DOWN) == true)
		{
			this->_player.stopLookingDown();
		}

		if (input.wasKeyPressed(SDL_SCANCODE_Z) == true)
		{
			this->_player.jump();
		}

		if ((!input.isKeyHeld(SDL_SCANCODE_LEFT) &&
				 !input.isKeyHeld(SDL_SCANCODE_RIGHT) &&
				 !input.isKeyHeld(SDL_SCANCODE_DOWN) &&
				 !input.isKeyHeld(SDL_SCANCODE_UP)))
		{
			this->_player.stopMoving();
		}

		const int CURRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;

		this->_graphics = graphics;
		this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CURRENT_TIME_MS;

		this->draw(graphics);
	}
}

void Game::draw(Graphics &graphics)
{
	graphics.clear();

	this->_level.draw(graphics, this->_player);

	this->_hud.draw(graphics);

	this->_player.draw(graphics);
	graphics.flip();
}

void Game::update(float elapsedTime)
{
	this->_player.update(elapsedTime);
	this->_level.update(elapsedTime, this->_player);
	this->_hud.update(elapsedTime, this->_player);

	// Check collisions
	std::vector<Rectangle> others;
	if ((others = this->_level.checkTileCollisions(this->_player.getBoundingBox())).size() > 0)
	{
		// Player collided with at least one tile. Handle it.
		this->_player.handleTileCollisions(others);
	}
	// Check slopes
	std::vector<Slope> otherSlopes;
	if ((otherSlopes = this->_level.checkSlopeCollisions(this->_player.getBoundingBox())).size() > 0)
	{
		this->_player.handleSlopeCollisions(otherSlopes);
	}
	std::vector<Door> doors;
	if ((doors = this->_level.checkDoorCollisions(this->_player.getBoundingBox())).size() > 0)
	{
		this->_player.handleDoorCollision(doors, this->_level, this->_graphics);
	}
	std::vector<LevelPassage> levelPassages;
	if ((levelPassages = this->_level.checkLevelPassage(this->_player.getBoundingBox())).size() > 0)
	{
		this->_player.handleLevelPassage(levelPassages, this->_level, this->_graphics);
	}
	std::vector<Enemy *> otherEnemies;
	if ((otherEnemies = this->_level.checkEnemyCollisions(this->_player.getBoundingBox())).size() > 0)
	{
		this->_player.handleEnemyCollisions(otherEnemies);
	}
}
