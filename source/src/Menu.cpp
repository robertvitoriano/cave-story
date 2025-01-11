#include "Menu.h"

void Menu::addItem(const std::string &text, std::function<void()> action)
{
  this->items.emplace_back(text, action);
}

void Menu::render(SDL_Renderer *renderer, int x, int y)
{
  for (size_t i = 0; i < this->items.size(); ++i)
  {
    this->items[i].render(renderer, x, y + static_cast<int>(i * 30), i == this->selectedIndex);
  }
}

void Menu::handleInput(SDL_Event &event)
{
  if (event.type == SDL_KEYDOWN)
  {
    switch (event.key.keysym.sym)
    {
    case SDLK_UP:
      navigateUp();
      break;
    case SDLK_DOWN:
      navigateDown();
      break;
    case SDLK_RETURN:
      executeSelected();
      break;
    default:
      break;
    }
  }
}

void Menu::navigateUp()
{
  if (this->selectedIndex > 0)
    this->selectedIndex--;
}

void Menu::navigateDown()
{
  if (this->selectedIndex < items.size() - 1)
    this->selectedIndex++;
}

void Menu::executeSelected()
{
  this->items[this->selectedIndex].executeAction();
}
