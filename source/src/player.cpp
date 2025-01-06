#include <player.h>
#include <graphics.h>
#include <MusicPlayer.h>
namespace player_constants
{
	const float WALK_SPEED = 0.2f;
	const float JUMP_SPEED = 0.7f;

	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;
}

Player::Player() {}

Player::Player(Graphics &graphics, Vector2 spawnPoint) : AnimatedSprite(graphics, "content/sprites/MyChar-no-bg.png", 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 100),
																												 _dx(0),
																												 _dy(0),
																												 _facing(RIGHT),
																												 _grounded(false),
																												 _lookingUp(false),
																												 _lookingDown(false),
																												 _maxHealth(3),
																												 _currentHealth(3),
																												 _shouldApplyGravity(true),
																												 _hitInterval(1000),
																												 _hitTimer(0),
																												 _isBlinking(false),
																												 _blinkStartTime(0),
																												 _blinkDuration(1000),
																												 _blinkInterval(100),
																												 _isPerformingAttack(false),
																												 _velocityIsEnabled(true),
																												 _collisionState(false, false)
{
	graphics.loadImage("content/sprites/MyChar-no-bg.png");

	this->setupAnimations();
	this->playAnimation("IdleRight");
}

void Player::setupAnimations()
{
	this->addAnimation(1, 0, 0, "IdleLeft", 16, 16, Vector2(0, 0), "row");
	this->addAnimation(1, 0, 16, "IdleRight", 16, 16, Vector2(0, 0), "row");
	this->addAnimation(3, 0, 0, "RunLeft", 16, 16, Vector2(0, 0), "row");
	this->addAnimation(3, 0, 16, "RunRight", 16, 16, Vector2(0, 0), "row");
	this->addAnimation(2, 7, 0, "RunUp", 16, 16, Vector2(0, 0), "column");
	this->addAnimation(1, 7, 0, "IdleUp", 16, 16, Vector2(0, 0), "row");
	this->addAnimation(2, 5, 0, "RunDown", 16, 16, Vector2(0, 0), "column");
	this->addAnimation(1, 5, 0, "IdleDown", 16, 16, Vector2(0, 0), "row");
	this->addAnimation(1, 3, 0, "IdleLeftUp", 16, 16, Vector2(0, 0), "row");
	this->addAnimation(1, 3, 16, "IdleRightUp", 16, 16, Vector2(0, 0), "row");
	this->addAnimation(3, 3, 0, "RunLeftUp", 16, 16, Vector2(0, 0), "row");
	this->addAnimation(3, 3, 16, "RunRightUp", 16, 16, Vector2(0, 0), "row");
	this->addAnimation(1, 6, 0, "LookDownLeft", 16, 16, Vector2(0, 0), "row");
	this->addAnimation(1, 6, 16, "LookDownRight", 16, 16, Vector2(0, 0), "row");
	this->addAnimation(1, 7, 0, "LookBackwardsLeft", 16, 16, Vector2(0, 0), "row");
	this->addAnimation(1, 7, 16, "LookBackwardsRight", 16, 16, Vector2(0, 0), "row");
}

void Player::animationDone(std::string currentAnimation) {}

const float Player::getX() const
{
	return this->_x;
}

const float Player::getY() const
{
	return this->_y;
}

void Player::moveLeft()
{
	if (this->_lookingDown == true && this->_grounded == true && this->_shouldApplyGravity)
	{
		return;
	}

	this->_dx = -player_constants::WALK_SPEED;

	if (this->_lookingUp == false)
	{
		MusicPlayer &musicPlayer = MusicPlayer::getInstance();
		musicPlayer.playSound("content/sounds/walk.wav", -1);
		this->playAnimation("RunLeft");
	}
	this->_facing = LEFT;
}

void Player::moveRight()
{
	if (this->_lookingDown == true && this->_grounded == true && this->_shouldApplyGravity)
	{
		return;
	}

	this->_dx = player_constants::WALK_SPEED;

	if (this->_lookingUp == false)
	{
		MusicPlayer &musicPlayer = MusicPlayer::getInstance();
		musicPlayer.playSound("content/sounds/walk.wav", -1);
		this->playAnimation("RunRight");
	}
	this->_facing = RIGHT;
}
void Player::moveUp()
{

	this->_dy = -player_constants::WALK_SPEED;

	MusicPlayer &musicPlayer = MusicPlayer::getInstance();
	musicPlayer.playSound("content/sounds/walk.wav", -1);
	this->playAnimation("RunUp");

	this->_facing = UP;
}
void Player::moveDown()
{

	this->_dy = player_constants::WALK_SPEED;

	MusicPlayer &musicPlayer = MusicPlayer::getInstance();
	musicPlayer.playSound("content/sounds/walk.wav", -1);
	this->playAnimation("RunDown");
	this->_facing = DOWN;
}
void Player::stopMoving()
{
	this->_dx = 0.0f;
	if (!this->isGravityEnabled())
	{
		this->_dy = 0.0f;
	}
	if (this->_lookingUp == false && this->_lookingDown == false)
	{
		switch (this->_facing)
		{
		case RIGHT:
			this->playAnimation("IdleRight");
			break;
		case LEFT:
			this->playAnimation("IdleLeft");
			break;

		case DOWN:
			this->playAnimation("IdleDown");
			break;

		case UP:
			this->playAnimation("IdleUp");
			break;

		default:
			this->playAnimation("IdleRight");
		}
		MusicPlayer &musicPlayer = MusicPlayer::getInstance();
		musicPlayer.stopSound("content/sounds/walk.wav");
	}
}

void Player::disableGravity()
{
	this->_shouldApplyGravity = false;
}

void Player::enableGravity()
{
	this->_shouldApplyGravity = true;
}

void Player::lookUp()
{
	this->_lookingUp = true;
	if (this->_dx == 0)
	{
		this->playAnimation(this->_facing == RIGHT ? "IdleRightUp" : "IdleLeftUp");
	}
	else
	{
		this->playAnimation(this->_facing == RIGHT ? "RunRightUp" : "RunLeftUp");
	}
}

void Player::stopLookingUp()
{
	this->_lookingUp = false;
}

void Player::lookDown()
{
	this->_lookingDown = true;
	if (this->_grounded == true)
	{
		// We need to interact (turn backwards)
		this->playAnimation(this->_facing == RIGHT ? "LookBackwardsRight" : "LookBackwardsLeft");
	}
	else
	{
		this->playAnimation(this->_facing == RIGHT ? "LookDownRight" : "LookDownLeft");
	}
}

void Player::stopLookingDown()
{
	this->_lookingDown = false;
}

void Player::jump()
{
	if (this->_grounded)
	{
		this->_dy = 0;
		this->_dy -= player_constants::JUMP_SPEED;
		this->_grounded = false;
	}
}

bool Player::isGravityEnabled()
{
	return this->_shouldApplyGravity;
}

void Player::handleTileCollisions(std::vector<Rectangle> &others)
{
	for (int i = 0; i < others.size(); i++)
	{
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE)
		{
			switch (collisionSide)
			{
			case sides::TOP:
				this->_dy = 0;
				this->_y = others.at(i).getBottom() + 1;
				if (this->_grounded)
				{
					this->_dx = 0;
					if (this->isGravityEnabled())
					{
						this->_x -= this->_facing == RIGHT ? 1.0f : -1.0f;
					}
				}
				break;
			case sides::BOTTOM:
				this->_y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
				this->_dy = 0;
				this->_grounded = true;
				break;
			case sides::LEFT:
				this->_x = others.at(i).getRight() + 1;
				this->_collisionState.horizontal = true;
				break;
			case sides::RIGHT:
				this->_collisionState.horizontal = true;
				this->_x = others.at(i).getLeft() - this->_boundingBox.getWidth() - 1;
				break;
			}
		}
	}
}

void Player::handleSlopeCollisions(std::vector<Slope> &others)
{
	for (int i = 0; i < others.size(); i++)
	{
		// Calculate where on the slope the player's bottom center is touching
		// and use y=mx+b to figure out the y position to place him at
		// First calculate "b" (slope intercept) using one of the points (b = y - mx)
		int b = (others.at(i).getP1().y - (others.at(i).getSlope() * fabs(others.at(i).getP1().x)));

		// Now get player's center x
		int centerX = this->_boundingBox.getCenterX();

		// Now pass that X into the equation y = mx + b (using our newly found b and x) to get the new y position
		int newY = (others.at(i).getSlope() * centerX) + b - 8; // 8 is temporary to fix a problem

		// Re-position the player to the correct "y"
		if (this->_grounded)
		{
			this->_y = newY - this->_boundingBox.getHeight();
			this->_grounded = true;
		}
	}
}

void Player::handleDoorCollision(std::vector<Door> &doors, Level &level, Graphics &graphics)
{
	for (int i = 0; i < doors.size(); i++)
	{
		if (this->_grounded == true && this->_lookingDown == true)
		{
			level = Level(doors.at(i).getDestination(), graphics);
			Camera &camera = Camera::getInstance();
			camera.follow(this, &level);
			bool positionIsValid = doors.at(i).getSpawnPosition().x != 0 && doors.at(i).getSpawnPosition().y != 0;
			if (positionIsValid)
			{

				this->_x = doors.at(i).getSpawnPosition().x;
				this->_y = doors.at(i).getSpawnPosition().y;
			}
			else
			{
				this->_x = level.getPlayerSpawnPoint().x;
				this->_y = level.getPlayerSpawnPoint().y;
			}

			if (doors.at(i).shouldEnableGravity())
			{
				this->enableGravity();
			}
			else
			{
				this->disableGravity();
			}
		}
	}
}

void Player::handle3DimensionalLevel()
{
	std::cout << "Handle 3d Level" << std::endl;
}

void Player::handleGravityChange(std::vector<GravityChange> &gravityChangers)
{
	for (int i = 0; i < gravityChangers.size(); i++)
	{
		if (gravityChangers.at(i).shouldEnableGravity())
		{
			this->enableGravity();
		}
		else
		{
			this->disableGravity();
		}
	}
}
void Player::handleLevelPassage(std::vector<LevelPassage> &levelPassages, Level &level, Graphics &graphics)
{
	for (int i = 0; i < levelPassages.size(); i++)
	{
		level = Level(levelPassages.at(i).getDestination(), graphics);
		Camera &camera = Camera::getInstance();
		camera.follow(this, &level);
		if (levelPassages.at(i).getSpawnPosition().x != 0 && levelPassages.at(i).getSpawnPosition().y != 0)
		{

			this->_x = levelPassages.at(i).getSpawnPosition().x;
			this->_y = levelPassages.at(i).getSpawnPosition().y;

			if (levelPassages.at(i).getSpawnPosition().x >= globals::SCREEN_WIDTH * 0.8)
			{
				camera.setOffset(Vector2(camera.getMaxXScroll(), camera.getY()));
			}
		}
		else
		{
			this->_x = level.getPlayerSpawnPoint().x;
			this->_y = level.getPlayerSpawnPoint().y;
		}
		if (levelPassages.at(i).shouldEnableGravity())
		{
			this->enableGravity();
		}
		else
		{
			this->disableGravity();
		}
	}
}

void Player::handleEnemyCollisions(std::vector<Enemy *> &enemies)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies.at(i)->touchPlayer(this);
	}
}

void Player::gainHealth(int amount)
{

	if (SDL_GetTicks() >= this->_hitTimer)
	{
		this->_currentHealth += amount;
		this->_hitTimer = SDL_GetTicks() + this->_hitInterval;
		if (amount < 0)
		{
			this->_isBlinking = true;
			this->_blinkStartTime = SDL_GetTicks();
		}
	}
}

void Player::update(float elapsedTime)
{
	if (this->_shouldApplyGravity && this->_dy <= player_constants::GRAVITY_CAP)
	{
		this->_dy += player_constants::GRAVITY * elapsedTime;
	}

	if (this->isVelocityEnabled())
	{
		this->_x += this->_dx * elapsedTime;
	}

	this->_y += this->_dy * elapsedTime;

	AnimatedSprite::update(elapsedTime);

	this->_currentWeapon.update(elapsedTime);
}

void Player::disableVelocity()
{
	this->_velocityIsEnabled = false;
}
void Player::enableVelocity()
{
	this->_velocityIsEnabled = true;
}
bool Player::isVelocityEnabled()
{
	return this->_velocityIsEnabled;
}
void Player::renderBlinkingPlayer(Graphics &graphics)
{

	Uint32 currentTime = SDL_GetTicks();
	if ((currentTime - this->_blinkStartTime) / this->_blinkInterval % 2 == 0)
	{
		AnimatedSprite::draw(graphics, this->_x, this->_y);
	}
	else
	{
		SDL_SetTextureColorMod(this->_spriteSheet, 255, 0, 0);
		AnimatedSprite::draw(graphics, this->_x, this->_y);
		SDL_SetTextureColorMod(this->_spriteSheet, 255, 255, 255);
	}

	if (currentTime > this->_blinkStartTime + this->_blinkDuration)
	{
		_isBlinking = false;
	}
}
void Player::draw(Graphics &graphics)
{

	if (_isBlinking)
	{
		this->renderBlinkingPlayer(graphics);
		return;
	}
	AnimatedSprite::draw(graphics, this->_x, this->_y);
	this->_currentWeapon.draw(graphics, this->_x, this->_y, this->_facing);
}

void Player::setWeapon(Sword &weapon)
{
	this->_currentWeapon = weapon;
}

void Player::attack()
{
	this->_isPerformingAttack = true;
	this->_currentWeapon.performAttack();
}

void Player::stopAttack()
{
	this->_currentWeapon.stopAttack();
	this->_isPerformingAttack = false;
}
bool Player::isAttacking()
{
	return this->_isPerformingAttack;
}
float Player::getXVelocity()
{
	return this->_dx;
}

float Player::getYVelocity()
{
	return this->_dy;
}
Direction Player::getFacing()
{
	return this->_facing;
}

CollisionState Player::getCollisionState()
{
	return this->_collisionState;
}

void Player::setCollisionState(CollisionState collisionState)
{
	this->_collisionState = collisionState;
}
