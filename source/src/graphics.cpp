
#include <graphics.h>

/* Graphics class
 * Holds all information dealing with graphics for the game
 */

Graphics::Graphics()
{

	SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, 0, &this->_window, &this->_renderer);
	SDL_SetWindowTitle(this->_window, "Cavestory");
	if (TTF_Init() == -1)
	{
		throw std::runtime_error("Failed to initialize SDL_ttf: " + std::string(TTF_GetError()));
	}
}

Graphics::~Graphics()
{
	SDL_DestroyWindow(this->_window);
	SDL_DestroyRenderer(this->_renderer);
}

SDL_Surface *Graphics::loadImage(const std::string &filePath)
{
	if (this->_spriteSheets.count(filePath) == 0)
	{
		this->_spriteSheets[filePath] = IMG_Load(filePath.c_str());
	}
	return this->_spriteSheets[filePath];
}

void Graphics::blitSurface(SDL_Texture *texture, SDL_Rect *sourceRectangle, SDL_Rect *destinationRectangle)
{
	SDL_RenderCopy(this->_renderer, texture, sourceRectangle, destinationRectangle);
}

void Graphics::flip()
{
	SDL_RenderPresent(this->_renderer);
}

void Graphics::clear()
{
	SDL_RenderClear(this->_renderer);
}

void Graphics::drawText(std::string text, SDL_Color color, Vector2 position)
{
	TTF_Font *font = TTF_OpenFont("content/fonts/BigBlueTermPlusNerdFontPropo-Regular.ttf", 24);
	if (!font)
	{
		SDL_Log("Failed to load font: %s", TTF_GetError());
		return;
	}

	SDL_Surface *textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
	if (!textSurface)
	{
		SDL_Log("Failed to create text surface: %s", TTF_GetError());
		TTF_CloseFont(font);
		return;
	}

	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(this->_renderer, textSurface);
	SDL_FreeSurface(textSurface);

	if (!textTexture)
	{
		SDL_Log("Failed to create text texture: %s", SDL_GetError());
		TTF_CloseFont(font);
		return;
	}

	SDL_Rect textRect = {position.x - textSurface->w / 2, position.y - textSurface->h / 2, textSurface->w,
											 textSurface->h};

	this->blitSurface(textTexture, nullptr, &textRect);
	SDL_DestroyTexture(textTexture);
	TTF_CloseFont(font);
}
SDL_Renderer *Graphics::getRenderer() const
{
	return this->_renderer;
}
