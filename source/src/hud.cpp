#include "hud.h"

#include "graphics.h"

HUD::HUD() {}

HUD::HUD(Graphics &graphics, Player &player)
{
	this->_player = player;
	this->_healthBarSprite = Sprite(graphics, "content/sprites/TextBox.png", 0, 40, 64, 8, 35, 70);
	this->_healthNumber1 = Sprite(graphics, "content/sprites/TextBox.png", 0, 56, 8, 8, 66, 70);
	this->_currentHealthBar = Sprite(graphics, "content/sprites/TextBox.png", 0, 25, 39, 5, 83, 72);
}

void HUD::update(int elapsedTime, Player &player)
{
	this->_player = player;

	this->_healthNumber1.setSourceRectX(8 * this->_player.getCurrentHealth());

	// Calculate the width of the health bar
	// 100% = 39 px
	float num = (float)this->_player.getCurrentHealth() / this->_player.getMaxHealth();
	this->_currentHealthBar.setSourceRectW(std::floor(num * 39));
}

void HUD::draw(Graphics &graphics)
{
	this->_healthBarSprite.draw(graphics, this->_healthBarSprite.getX(), this->_healthBarSprite.getY());
	this->_healthNumber1.draw(graphics, this->_healthNumber1.getX(), this->_healthNumber1.getY());
	this->_currentHealthBar.draw(graphics, this->_currentHealthBar.getX(), this->_currentHealthBar.getY());
}
