#include "MenuItem.h"

MenuItem::MenuItem(const std::string &text, std::function<void()> action)
    : text(text), action(action) {}

void MenuItem::render(SDL_Renderer *renderer, int x, int y, bool selected)
{
  SDL_Color color = selected ? SDL_Color{255, 255, 0, 255} : SDL_Color{255, 255, 255, 255};
}

void MenuItem::executeAction()
{
  if (action)
    action();
}
