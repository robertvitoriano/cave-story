#ifndef MENU_ITEM
#define MENU_ITEM
#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <functional>
class MenuItem
{
public:
  MenuItem(const std::string &text, std::function<void()> action);

  void render(SDL_Renderer *renderer, int x, int y, bool selected);
  void executeAction();

private:
  std::string text;
  std::function<void()> action;
};
#endif
