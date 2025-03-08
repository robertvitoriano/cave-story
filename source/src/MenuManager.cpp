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

void MenuManager::render(Graphics &graphics)
{
  if (currentMenu)
    currentMenu->render(graphics, 100, 100);
}
