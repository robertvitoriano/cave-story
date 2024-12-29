
#include "game.h"
#include "graphics.h"
#include "weapon.h"
namespace
{
	const int FPS = 50;
	const int MAX_FRAME_TIME = 1000 / FPS;
}

Game::Game() : gameIsLost(false)
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
	Sword sword = Sword(graphics, this->_level.getPlayerSpawnPoint());

	this->_player.setWeapon(sword);

	int LAST_UPDATE_TIME = SDL_GetTicks();

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
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				input.mouseButtonDownEvent(event);
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				input.mouseButtonUpEvent(event);
			}
			else if (event.type == SDL_QUIT)
			{
				return;
			}
		}

		handleInput(input);

		const int CURRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;

		this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME), graphics);
		LAST_UPDATE_TIME = CURRENT_TIME_MS;

		this->draw(graphics);
	}
}

void Game::handleInput(Input &input)
{
	if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE))
	{
		return;
	}
	if (input.isKeyHeld(SDL_SCANCODE_F) || input.isMouseButtonHeld(SDL_BUTTON_LEFT))
	{
		this->_player.attack();
	}
	else if (input.isKeyHeld(SDL_SCANCODE_A))
	{
		this->_player.moveLeft();
	}
	else if (input.isKeyHeld(SDL_SCANCODE_D))
	{
		this->_player.moveRight();
	}
	else if (input.isKeyHeld(SDL_SCANCODE_W) && !this->_player.isGravityEnabled())
	{
		this->_player.moveUp();
	}
	else if (input.isKeyHeld(SDL_SCANCODE_S) && !this->_player.isGravityEnabled())
	{
		this->_player.moveDown();
	}

	if (input.isKeyHeld(SDL_SCANCODE_W) && this->_player.isGravityEnabled())
	{
		this->_player.lookUp();
	}
	else if (input.isKeyHeld(SDL_SCANCODE_S) && this->_player.isGravityEnabled())
	{
		this->_player.lookDown();
	}

	if (input.wasKeyReleased(SDL_SCANCODE_W))
	{
		this->_player.stopLookingUp();
	}
	if (input.wasKeyReleased(SDL_SCANCODE_S))
	{
		this->_player.stopLookingDown();
	}
	if (input.wasKeyReleased(SDL_SCANCODE_F) || input.wasMouseButtonReleased(SDL_BUTTON_LEFT))
	{
		this->_player.stopAttack();
	}

	if (input.wasKeyPressed(SDL_SCANCODE_SPACE))
	{
		this->_player.jump();
	}

	if (!input.isKeyHeld(SDL_SCANCODE_A) &&
			!input.isKeyHeld(SDL_SCANCODE_D) &&
			!input.isKeyHeld(SDL_SCANCODE_S) &&
			!input.isKeyHeld(SDL_SCANCODE_W))
	{
		this->_player.stopMoving();
	}
}

void Game::draw(Graphics &graphics)
{
	graphics.clear();

	if (!gameIsLost)
	{

		this->_level.draw(graphics, this->_player);

		this->_hud.draw(graphics);

		this->_player.draw(graphics);
	}
	else
	{
		SDL_Color white = {255, 255, 255, 255};
		Vector2 position = {globals::SCREEN_WIDTH / 2, globals::SCREEN_HEIGHT / 2};
		graphics.drawText("You Lost", white, position);
	}
	graphics.flip();
}

void Game::update(float elapsedTime, Graphics &graphics)
{
	this->_player.update(elapsedTime);
	this->_level.update(elapsedTime, this->_player);
	this->_hud.update(elapsedTime, this->_player);

	if (this->_player.getCurrentHealth() == 0)
	{
		gameIsLost = true;
	}

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
		this->_player.handleDoorCollision(doors, this->_level, graphics);
	}
	std::vector<LevelPassage> levelPassages;
	if ((levelPassages = this->_level.checkLevelPassage(this->_player.getBoundingBox())).size() > 0)
	{
		this->_player.handleLevelPassage(levelPassages, this->_level, graphics);
	}
	std::vector<GravityChange> gravityChangers;
	if ((gravityChangers = this->_level.checkGravityChange(this->_player.getBoundingBox())).size() > 0)
	{
		this->_player.handleGravityChange(gravityChangers);
	}
	std::vector<Enemy *> otherEnemies;
	if ((otherEnemies = this->_level.checkEnemyCollisions(this->_player.getBoundingBox())).size() > 0)
	{
		this->_player.handleEnemyCollisions(otherEnemies);
	}
}
