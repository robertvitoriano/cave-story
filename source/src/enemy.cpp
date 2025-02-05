#include <enemy.h>

Enemy::Enemy() {}

Enemy::Enemy(Graphics &graphics, std::string filePath, int sourceX, int sourceY,
						 int width, int height, Vector2 spawnPoint, int timeToUpdate)
		: AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height,
										 spawnPoint.x, spawnPoint.y, timeToUpdate),
			_direction(LEFT), _maxHealth(0), _currentHealth(0)
{
}

void Enemy::update(int elapsedTime, Player &player)
{
	AnimatedSprite::update(elapsedTime);
}

void Enemy::draw(Graphics &graphics, Vector2 offset)
{
	AnimatedSprite::draw(graphics, this->_x + offset.x, this->_y + offset.y);
}
