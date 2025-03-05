#include <animatedsprite.h>
#include <graphics.h>
#include <sprite.h>

/* AnimatedSprite class
 * Animates our sprites
 */

AnimatedSprite::AnimatedSprite() {}

AnimatedSprite::AnimatedSprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY,
															 int width, int height, float posX, float posY, float timeToUpdate) : Sprite(graphics, filePath, sourceX, sourceY, width, height, posX, posY),
																																																		_frameIndex(0),
																																																		_timeToUpdate(timeToUpdate),
																																																		_visible(true),
																																																		_currentAnimationOnce(false),
																																																		_currentAnimation("")
{
}

void AnimatedSprite::addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset, std::string orientation = "row")
{

	std::vector<SDL_Rect> rectangles;
	if (orientation == "row")
	{
		for (int i = 0; i < frames; i++)
		{
			SDL_Rect newRect = {(i + x) * width, y, width, height};
			rectangles.push_back(newRect);
		}
	}
	else if (orientation == "column")
	{
		for (int i = 0; i < frames; i++)
		{
			SDL_Rect newRect = {x * width, (i + y) * width, width, height};
			rectangles.push_back(newRect);
		}
	}
	this->_animations.insert(std::pair<std::string, std::vector<SDL_Rect>>(name, rectangles));
	this->_offsets.insert(std::pair<std::string, Vector2>(name, offset));
}

void AnimatedSprite::resetAnimations()
{
	this->_animations.clear();
	this->_offsets.clear();
}

void AnimatedSprite::playAnimation(std::string animation, bool once)
{
	this->_currentAnimationOnce = once;
	if (this->_currentAnimation != animation)
	{
		this->_currentAnimation = animation;
		this->_frameIndex = 0;
	}
}

void AnimatedSprite::setVisible(bool visible)
{
	this->_visible = visible;
}

void AnimatedSprite::stopAnimation()
{
	this->_frameIndex = 0;
	this->animationDone(this->_currentAnimation);
}

void AnimatedSprite::update(int elapsedTime)
{
	Sprite::update();
	std::cout << "ELAPSED TIME " << elapsedTime << std::endl;
	std::cout << "TIME ELAPSED " << this->_timeElapsed << std::endl;
	std::cout << "TIME TO UPDATE " << this->_timeToUpdate << std::endl;
	this->_timeElapsed += elapsedTime;

	// std::cout << "CURRENT ANIMATION " << this->_currentAnimation << "  FRAME INDEX " << std::to_string(this->_frameIndex) << "CURRENT ANIMATION SIZE " << this->_animations[this->_currentAnimation].size() << std::endl;

	if (this->_timeElapsed > this->_timeToUpdate)
	{
		this->_timeElapsed -= this->_timeToUpdate;
		if (this->_frameIndex < this->_animations[this->_currentAnimation].size() - 1)
		{
			this->_frameIndex++;
		}
		else
		{
			if (this->_currentAnimationOnce == true)
			{
				this->setVisible(false);
			}
			this->stopAnimation();
		}
	}
}

void AnimatedSprite::draw(Graphics &graphics, int x, int y)
{

	if (this->_visible)
	{
		SDL_Rect destinationRectangle;
		destinationRectangle.x = x + this->_offsets[this->_currentAnimation].x;
		destinationRectangle.y = y + this->_offsets[this->_currentAnimation].y;
		destinationRectangle.w = this->_sourceRect.w * globals::SPRITE_SCALE;
		destinationRectangle.h = this->_sourceRect.h * globals::SPRITE_SCALE;

		SDL_Rect sourceRect = this->_animations[this->_currentAnimation][this->_frameIndex];
		graphics.blitSurface(this->_spriteSheet, &sourceRect, &destinationRectangle);
	}
}
