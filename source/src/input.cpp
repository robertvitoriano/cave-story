#include <input.h>

void Input::beginNewFrame()
{
	this->_pressedKeys.clear();
	this->_releasedKeys.clear();
	this->_pressedMouseButtons.clear();
	this->_releasedMouseButtons.clear();
	// this->_heldJoystickButtons.clear();
}

void Input::keyDownEvent(const SDL_Event &event)
{
	this->_pressedKeys[event.key.keysym.scancode] = true;
	this->_heldKeys[event.key.keysym.scancode] = true;
}
void Input::keyUpEvent(const SDL_Event &event)
{
	this->_releasedKeys[event.key.keysym.scancode] = true;
	this->_heldKeys[event.key.keysym.scancode] = false;
}
void Input::joystickButtonDownEvent(int button)
{
	this->_pressedJoystickButtons[button] = true;
	this->_heldJoystickButtons[button] = true;
}
void Input::joystickButtonUpEvent(int joystickButton)
{
	this->_releasedJoystickButtons[joystickButton] = true;
	this->_heldJoystickButtons[joystickButton] = false;
	this->_pressedJoystickButtons[joystickButton] = true;
}
void Input::AxisMovedEvent(const SDL_Event &event)
{
	this->_pressedAxis[event.jaxis.axis] = event.jaxis.value;
	this->_heldAxis[event.jaxis.axis] = event.jaxis.value;
}

void Input::mouseButtonDownEvent(const SDL_Event &event)
{
	this->_pressedMouseButtons[event.button.button] = true;
	this->_heldMouseButtons[event.button.button] = true;
}

void Input::mouseButtonUpEvent(const SDL_Event &event)
{
	this->_releasedMouseButtons[event.button.button] = true;
	this->_heldMouseButtons[event.button.button] = false;
}

bool Input::wasKeyPressed(SDL_Scancode key)
{
	return this->_pressedKeys[key];
}

bool Input::wasKeyReleased(SDL_Scancode key)
{
	return this->_releasedKeys[key];
}

bool Input::isKeyHeld(SDL_Scancode key)
{
	return this->_heldKeys[key];
}
bool Input::wasJoystickButtonPressed(int joystickButton)
{
	return this->_pressedJoystickButtons[joystickButton];
}

bool Input::wasJoystickButtonReleased(int joystickButton)
{
	return this->_releasedJoystickButtons[joystickButton];
}

bool Input::isJoystickButtonHeld(int joystickButton)
{
	return this->_heldJoystickButtons[joystickButton];
}
Sint16 Input::wasAxisReleased(int axis)
{
	return this->_releasedAxis[axis];
}
Sint16 Input::isAxisHeld(int axis)
{
	return this->_heldAxis[axis];
}
Sint16 Input::wasAxisPressed(int axis)
{
	return this->_pressedAxis[axis];
}
bool Input::wasMouseButtonPressed(Uint8 button)
{
	return this->_pressedMouseButtons[button];
}

bool Input::wasMouseButtonReleased(Uint8 button)
{
	return this->_releasedMouseButtons[button];
}

bool Input::isMouseButtonHeld(Uint8 button)
{
	return this->_heldMouseButtons[button];
}
