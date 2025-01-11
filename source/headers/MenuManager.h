#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H
#include <SDL2/SDL.h>
#include <Menu.h>
#include <memory>
class MenuManager
{
public:
  void setMenu(std::shared_ptr<Menu> menu);
  void handleInput(SDL_Event &event);
  void render(Graphics &graphics);

private:
  std::shared_ptr<Menu> currentMenu;
};

#endif
