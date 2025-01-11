#include "MenuItem.h"

MenuItem::MenuItem(const std::string &text, std::function<void()> action)
    : text(text), action(action) {}

void MenuItem::render(Graphics &graphics, int x, int y, bool selected)
{
  SDL_Color color = selected ? SDL_Color{255, 255, 0, 255} : SDL_Color{255, 255, 255, 255};
  Vector2 position = {x, y};
  graphics.drawText(this->text, color, position);
}

void MenuItem::executeAction()
{
  if (action)
    action();
}
