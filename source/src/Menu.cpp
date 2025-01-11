#include "Menu.h"

void Menu::addItem(const std::string &text, std::function<void()> action)
{
  items.emplace_back(text, action);
}

void Menu::render(SDL_Renderer *renderer, int x, int y)
{
  for (size_t i = 0; i < items.size(); ++i)
  {
    items[i].render(renderer, x, y + static_cast<int>(i * 30), i == selectedIndex);
  }
}

void Menu::handleInput(SDL_Event &event)
{
  if (event.type == SDL_KEYDOWN)
  {
    if (event.key.keysym.sym == SDLK_UP)
    {
      if (selectedIndex > 0)
        selectedIndex--;
    }
    else if (event.key.keysym.sym == SDLK_DOWN)
    {
      if (selectedIndex < items.size() - 1)
        selectedIndex++;
    }
    else if (event.key.keysym.sym == SDLK_RETURN)
    {
      items[selectedIndex].executeAction();
    }
  }
}
