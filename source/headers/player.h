#ifndef PLAYER_H
#define PLAYER_H

#include "animatedsprite.h"
#include "globals.h"
#include "slope.h"
#include "level.h"
#include "enemy.h"
#include "sword.h"

class Graphics;

class Player : public AnimatedSprite
{
public:
	Player();
	Player(Graphics &graphics, Vector2 spawnPoint);
	void draw(Graphics &graphics);
	void update(float elapsedTime);

	/* void moveLeft
	 * Moves the player left by -dx
	 */
	void moveLeft();

	/* void moveRight
	 * Moves the player right by dx
	 */
	void moveRight();

	/* void moveDown
	 * Moves the player down by +dy
	 */
	void moveDown();

	/* void moveRight
	 * Moves the player right by -dy
	 */
	void moveUp();

	/* void stopMoving
	 * Stops moving the player
	 */
	void stopMoving();

	/* void jump
	 * Starts jumping
	 */

	/* void lookUp
	 * The player looks up
	 */
	void lookUp();

	/* void stopLookingUp
	 * The player stops looking up
	 */
	void stopLookingUp();

	/* void lookDown
	 * The player looks down OR interacts (turns around)
	 */
	void lookDown();

	/* void stopLookingDown
	 * The player stops looking down or interacting
	 */
	void stopLookingDown();

	void jump();

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();

	void handleTileCollisions(std::vector<Rectangle> &others);
	void handleSlopeCollisions(std::vector<Slope> &others);
	void handleDoorCollision(std::vector<Door> &others, Level &level, Graphics &graphics);
	void handleLevelPassage(std::vector<LevelPassage> &others, Level &level, Graphics &graphics);
	void handleEnemyCollisions(std::vector<Enemy *> &others);

	const float getX() const;
	const float getY() const;

	const inline int getMaxHealth() const { return this->_maxHealth; }
	const inline int getCurrentHealth() const { return this->_currentHealth; }

	void gainHealth(int amount);
	void disableGravity();
	void enableGravity();
	bool isGravityEnabled();
	void handle3DimensionalLevel();
	void renderBlinkingPlayer(Graphics &graphics);
	void setWeapon(Sword &weapon);

private:
	float _dx,
			_dy;

	Direction _facing;

	bool _grounded;
	bool _shouldApplyGravity;
	bool _lookingUp;
	bool _lookingDown;

	int _maxHealth;
	int _currentHealth;
	Uint32 _hitTimer;
	Uint32 _hitInterval;
	bool _isBlinking;
	Uint32 _blinkStartTime;
	int _blinkDuration;
	int _blinkInterval;
	Sword _currentWeapon;
};

#endif
