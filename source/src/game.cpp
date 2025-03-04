
#include <game.h>
#include <graphics.h>
#include <stdio.h>
namespace
{
	const int FPS = 50;
	const int MAX_FRAME_TIME = 1000 / FPS;
}

Game::Game() : _gameState(gameState::MAIN_MENU), _displayDebug(false), _joystick(nullptr), _mainMenu(std::make_shared<Menu>()), _pauseMenu(std::make_shared<Menu>())
{
	SDL_Init(SDL_INIT_EVERYTHING);
	MusicPlayer &musicPlayer = MusicPlayer::getInstance();
	musicPlayer.playSound("content/sounds/background.mp3", -1);
	int numberOfJoysticks = SDL_NumJoysticks();
	printf("%d joysticks found\n", numberOfJoysticks);
	if (numberOfJoysticks > 0)
	{
		this->_joystick = SDL_JoystickOpen(0);
		if (this->_joystick)
		{
			printf("Opened Joystick 0\n");
			printf("Name: %s\n", SDL_JoystickName(this->_joystick));
			printf("Number of Axes: %d\n", SDL_JoystickNumAxes(this->_joystick));
			printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(this->_joystick));
			printf("Number of Balls: %d\n", SDL_JoystickNumBalls(this->_joystick));
		}
		else
		{
			printf("Couldn't open Joystick 0\n");
		}
	}
	this->_mainMenu->addItem("Start Game", [this]()
													 { this->_gameState = gameState::PLAYING; });
	this->_mainMenu->addItem("Options", []()
													 { std::cout << " options SHOULD show" << std::endl; });
	this->_mainMenu->addItem("Exit", []()
													 { SDL_Quit(); exit(0); });

	this->_pauseMenu->addItem("Resume", [this]()
														{ this->_gameState = gameState::PLAYING; });
	this->_pauseMenu->addItem("Main menu", [this]()
														{ this->_gameState = gameState::MAIN_MENU; });

	this->_menuManager.setMenu(this->_mainMenu);
	this->gameLoop();
}

Game::~Game()
{
	if (this->_joystick)
		SDL_JoystickClose(this->_joystick);

	this->_joystick = nullptr;
}

void Game::gameLoop()
{
	Graphics graphics;
	Input input;
	SDL_Event event;
	this->_level = Level("Map_1", graphics);
	this->_player = Player(graphics, this->_level.getPlayerSpawnPoint());
	this->_hud = HUD(graphics, this->_player);
	Camera &camera = Camera::getInstance();
	camera.follow(&this->_player, &this->_level);

	int LAST_UPDATE_TIME = SDL_GetTicks();

	while (true)
	{
		input.beginNewFrame();

		if (SDL_PollEvent(&event))
		{
			if (this->_gameState == gameState::MAIN_MENU)
			{
				this->_mainMenu->handleInput(event);
			}
			if (this->_gameState == gameState::PAUSED)
			{
				this->_pauseMenu->handleInput(event);
			}
			switch (event.type)
			{
			case SDL_KEYDOWN:
				if (event.key.repeat == 0)
				{
					input.keyDownEvent(event);
				}
				break;

			case SDL_KEYUP:
				input.keyUpEvent(event);
				break;

			case SDL_MOUSEBUTTONDOWN:
				input.mouseButtonDownEvent(event);
				break;

			case SDL_MOUSEBUTTONUP:
				input.mouseButtonUpEvent(event);
				break;

			case SDL_QUIT:
				return;
			default:
				break;
			}
		}

		const int CURRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;

		handleInput(input, std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));

		this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME), graphics, input);
		LAST_UPDATE_TIME = CURRENT_TIME_MS;

		this->draw(graphics);
	}
}

void Game::handleInput(Input &input, float elapsedTime)
{
	Camera &camera = Camera::getInstance();

	if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE))
	{
		if (this->_gameState == gameState::PAUSED)
		{
			this->_gameState = gameState::PLAYING;
			return;
		}
		if (this->_gameState == gameState::PLAYING)
		{
			this->_gameState = gameState::PAUSED;
		}
	}
	if (input.isKeyHeld(SDL_SCANCODE_F) || input.isMouseButtonHeld(SDL_BUTTON_LEFT) || SDL_JoystickGetButton(this->_joystick, JoystickButtons::RECTANGLE))
	{
		this->_player.shoot();
	}
	else if (input.isKeyHeld(SDL_SCANCODE_A) || input.isJoystickButtonHeld(JoystickButtons::JOYSTICK_LEFT))
	{
		camera.moveRight();
		this->_player.moveLeft();
	}
	else if (input.isKeyHeld(SDL_SCANCODE_D) || input.isJoystickButtonHeld(JoystickButtons::JOYSTICK_RIGHT))
	{
		camera.moveLeft();
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
	else if ((input.isKeyHeld(SDL_SCANCODE_S) || input.isJoystickButtonHeld(JoystickButtons::JOYSTICK_DOWN)) && this->_player.isGravityEnabled())
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

	if (input.wasKeyPressed(SDL_SCANCODE_SPACE) || SDL_JoystickGetButton(this->_joystick, JoystickButtons::X))
	{
		this->_player.jump();
	}
	if (input.wasKeyPressed(SDL_SCANCODE_J))
	{
		this->toggleDebug();
	}

	if (!input.isKeyHeld(SDL_SCANCODE_A) &&
			!input.isKeyHeld(SDL_SCANCODE_D) &&
			!input.isKeyHeld(SDL_SCANCODE_S) &&
			!input.isKeyHeld(SDL_SCANCODE_W) &&
			!input.isJoystickButtonHeld(JoystickButtons::JOYSTICK_LEFT) &&
			!input.isJoystickButtonHeld(JoystickButtons::JOYSTICK_UP) &&
			!input.isJoystickButtonHeld(JoystickButtons::JOYSTICK_RIGHT) &&
			!input.isJoystickButtonHeld(JoystickButtons::JOYSTICK_DOWN))
	{
		this->_player.stopMoving();
		camera.stopMoving();
	}
}

void Game::draw(Graphics &graphics)
{
	graphics.clear();

	switch (this->_gameState)
	{
	case gameState::MAIN_MENU:
		this->_mainMenu->render(graphics, globals::SCREEN_WIDTH / 2, globals::SCREEN_HEIGHT / 2);
		break;
	case gameState::PLAYING:
		this->drawGame(graphics);
		break;
	case gameState::PAUSED:
		this->drawGame(graphics);
		this->_pauseMenu->render(graphics, globals::SCREEN_WIDTH / 2, globals::SCREEN_HEIGHT / 2);
		break;
	case gameState::LOST:
		SDL_Color white = {255, 255, 255, 255};
		Vector2 position = {globals::SCREEN_WIDTH / 2, globals::SCREEN_HEIGHT / 2};
		graphics.drawText("You Lost", white, position);
		break;
	}

	graphics.flip();
}
void Game::drawGame(Graphics &graphics)
{
	Camera &camera = Camera::getInstance();

	this->_level.draw(graphics, this->_player);

	this->_hud.draw(graphics);

	this->_player.draw(graphics);
	if (this->_displayDebug)
	{
		this->_level.drawDebug(graphics);
		camera.drawDebug(graphics);
	}
}
void Game::toggleDebug()
{
	this->_displayDebug = !this->_displayDebug;
}

void Game::update(float elapsedTime, Graphics &graphics, Input &input)
{
	switch (this->_gameState)
	{
	case gameState::PLAYING:
		this->playingUpdate(elapsedTime, graphics, input);
		break;
	}
}

void Game::playingUpdate(float elapsedTime, Graphics &graphics, Input &input)
{
	this->_player.update(elapsedTime);
	this->_level.update(elapsedTime, this->_player);
	this->_hud.update(elapsedTime, this->_player);
	Camera &camera = Camera::getInstance();

	camera.update(elapsedTime);
	// printf("Checking joystick button presses...\n");
	for (int i = 0; i < SDL_JoystickNumButtons(this->_joystick); i++)
	{
		if (SDL_JoystickGetButton(this->_joystick, i))
		{
			input.joystickButtonDownEvent(i);
		}
		else
		{
			input.joystickButtonUpEvent(i);
		}
	}

	if (this->_player.getY() >= globals::SCREEN_HEIGHT || this->_player.getCurrentHealth() == 0)
	{
		this->_gameState = gameState::LOST;
	}

	std::vector<Rectangle> others;
	if ((others = this->_level.checkTileCollisions(this->_player.getBoundingBox())).size() > 0)
	{
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
