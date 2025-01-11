
#include <game.h>
#include <graphics.h>
#include <weapon.h>
#include <stdio.h>
namespace
{
	const int FPS = 50;
	const int MAX_FRAME_TIME = 1000 / FPS;
}

Game::Game() : gameIsLost(false), _displayDebug(false), _joystick(nullptr)
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
	this->gameLoop();
	MenuManager menuManager;

	auto mainMenu = std::make_shared<Menu>();
	mainMenu->addItem("Start Game", []() {});
	mainMenu->addItem("Options", []() {});
	mainMenu->addItem("Exit", []()
										{ SDL_Quit(); exit(0); });

	menuManager.setMenu(mainMenu);
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
	Sword sword = Sword(graphics, this->_level.getPlayerSpawnPoint());

	this->_player.setWeapon(sword);

	int LAST_UPDATE_TIME = SDL_GetTicks();

	while (true)
	{
		input.beginNewFrame();

		if (SDL_PollEvent(&event))
		{
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

				// case SDL_JOYAXISMOTION:
				// 	if ((event.jaxis.value < -3200) || (event.jaxis.value > 3200))
				// 	{
				// 		if (event.jaxis.axis == 0)
				// 		{
				// 			if (event.jaxis.value > 0)
				// 			{
				// 				this->_player.moveRight();
				// 			}
				// 			else if (event.jaxis.value < 0)
				// 			{
				// 				this->_player.moveLeft();
				// 			}
				// 		}
				// 		else if (event.jaxis.axis == 1)
				// 		{
				// 			if (event.jaxis.value < 0)
				// 			{
				// 				this->_player.moveUp();
				// 			}
				// 			else if (event.jaxis.value > 0)
				// 			{
				// 				this->_player.moveDown();
				// 			}
				// 		}
				// 	}
				// 	break;

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
		return;
	}
	if (input.isKeyHeld(SDL_SCANCODE_F) || input.isMouseButtonHeld(SDL_BUTTON_LEFT) || SDL_JoystickGetButton(this->_joystick, JoystickButtons::RECTANGLE))
	{
		this->_player.attack();
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
	if (input.wasKeyReleased(SDL_SCANCODE_F) ||
			input.wasMouseButtonReleased(SDL_BUTTON_LEFT) ||
			input.wasJoystickButtonReleased(JoystickButtons::RECTANGLE))
	{
		this->_player.stopAttack();
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
	Camera &camera = Camera::getInstance();

	graphics.clear();

	if (!gameIsLost)
	{

		this->_level.draw(graphics, this->_player);

		this->_hud.draw(graphics);

		this->_player.draw(graphics);
		if (this->_displayDebug)
		{
			this->_level.drawDebug(graphics);
			camera.drawDebug(graphics);
		}
	}
	else
	{
		SDL_Color white = {255, 255, 255, 255};
		Vector2 position = {globals::SCREEN_WIDTH / 2, globals::SCREEN_HEIGHT / 2};
		graphics.drawText("You Lost", white, position);
	}
	graphics.flip();
}
void Game::toggleDebug()
{
	this->_displayDebug = !this->_displayDebug;
}

void Game::update(float elapsedTime, Graphics &graphics, Input &input)
{
	this->_player.update(elapsedTime);
	this->_level.update(elapsedTime, this->_player);
	this->_hud.update(elapsedTime, this->_player);
	Camera &camera = Camera::getInstance();

	camera.update(elapsedTime);
	printf("Checking joystick button presses...\n");
	for (int i = 0; i < SDL_JoystickNumButtons(this->_joystick); i++)
	{
		if (SDL_JoystickGetButton(this->_joystick, i))
		{
			input.joystickButtonDownEvent(i);
			std::cout << "PRESSED " << i << std::endl;
		}
		else
		{
			input.joystickButtonUpEvent(i);
		}
	}

	if (this->_player.getCurrentHealth() == 0)
	{
		gameIsLost = true;
	}

	if (this->_player.getY() >= globals::SCREEN_HEIGHT)
	{
		gameIsLost = true;
	}

	// Check collisions
	std::vector<Rectangle> others;
	if ((others = this->_level.checkTileCollisions(this->_player.getBoundingBox())).size() > 0)
	{
		this->_player.handleTileCollisions(others);
	}
	else
	{
		// CollisionState collisionState(false, false);
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
