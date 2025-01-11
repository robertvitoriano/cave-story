#include "MenuManager.h"

void MenuManager::setMenu(std::shared_ptr<Menu> menu)
{
  currentMenu = menu;
}

void MenuManager::handleInput(SDL_Event &event)
{
  if (currentMenu)
    currentMenu->handleInput(event);
}

void MenuManager::render(SDL_Renderer *renderer)
{
  if (currentMenu)
    currentMenu->render(renderer, 100, 100); // Example starting position
}
