#ifndef MENU_ITEM
#define MENU_ITEM
#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include <functional>
#include <graphics.h>
#include <globals.h>
class MenuItem
{
public:
  MenuItem(const std::string &text, std::function<void()> action);

  void render(Graphics &graphics, int x, int y, bool selected);
  void executeAction();

private:
  std::string text;
  std::function<void()> action;
};
#endif
