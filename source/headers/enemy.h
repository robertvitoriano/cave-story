#ifndef ENEMY_H
#define ENEMY_H

#include "animatedsprite.h"
#include "player.h"	 // Include Player class for reference
#include "globals.h" // Assuming it contains global definitions like Direction

class Graphics; // Forward declaration of Graphics class

class Enemy : public AnimatedSprite
{
public:
	Enemy();
	Enemy(Graphics &graphics, std::string filePath, int sourceX, int sourceY,
				int width, int height, Vector2 spawnPoint, int timeToUpdate);

	virtual void update(int elapsedTime, Player &player);
	virtual void draw(Graphics &graphics);
	virtual void touchPlayer(Player *player) = 0; // Pure virtual function
	virtual bool shouldBeDestroyed() = 0;

protected:
	Direction _direction;
	int _maxHealth;
	int _currentHealth;
};

#endif // ENEMY_H
