#ifndef MENU_H
#define MENU_H
#include <MenuItem.h>
#include <graphics.h>
class Menu
{
public:
  void addItem(const std::string &text, std::function<void()> action);
  void render(Graphics &graphics, int x, int y);
  void handleInput(SDL_Event &event);
  void navigateUp();
  void navigateDown();
  void executeSelected();

private:
  std::vector<MenuItem> items;
  size_t selectedIndex = 0;
};

#endif
