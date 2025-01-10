#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <map>

class Input
{
public:
	void beginNewFrame();
	void keyUpEvent(const SDL_Event &event);
	void keyDownEvent(const SDL_Event &event);

	void mouseButtonDownEvent(const SDL_Event &event);
	void mouseButtonUpEvent(const SDL_Event &event);

	void joystickButtonDownEvent(int button);
	void joystickButtonUpEvent(int button);

	bool wasKeyPressed(SDL_Scancode key);
	bool wasKeyReleased(SDL_Scancode key);
	bool isKeyHeld(SDL_Scancode key);

	bool wasMouseButtonPressed(Uint8 button);
	bool wasMouseButtonReleased(Uint8 button);

	bool wasJoystickButtonPressed(int JoystickButton);
	bool wasJoystickButtonReleased(int JoystickButton);
	bool isJoystickButtonHeld(int JoystickButton);

	bool isMouseButtonHeld(Uint8 button);
	void AxisMovedEvent(const SDL_Event &event);
	Sint16 wasAxisReleased(int axis);
	Sint16 isAxisHeld(int axis);
	Sint16 wasAxisPressed(int axis);

private:
	std::map<SDL_Scancode, bool> _heldKeys;
	std::map<SDL_Scancode, bool> _pressedKeys;
	std::map<SDL_Scancode, bool> _releasedKeys;

	std::map<Uint8, bool> _heldMouseButtons;
	std::map<Uint8, bool> _pressedMouseButtons;
	std::map<Uint8, bool> _releasedMouseButtons;

	std::map<Uint8, bool> _pressedJoystickButtons;
	std::map<Uint8, bool> _heldJoystickButtons;
	std::map<Uint8, bool> _releasedJoystickButtons;

	std::map<Uint8, Sint16> _heldAxis;
	std::map<Uint8, Sint16> _pressedAxis;
	std::map<Uint8, Sint16> _releasedAxis;
};

#endif
