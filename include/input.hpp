#include <SDL2/SDL.h>
#include "ivec2.hpp"

class Game;

class ButtonState {
public:
	bool pressed;
	bool on_edge;

	bool GetPressed() {return pressed;}
	bool GetOnEdge() {return on_edge;}
};

class InputManager {
public:
	InputManager(Game* game);
	~InputManager();

	void Update();

	ButtonState QueryKey(int scancode);	
	ButtonState QueryMouseButton(int index);
	
	ivec2 GetMousePos();
	ivec2 GetMouseDelta();

	void SetMouseCaptured(bool captured);
private:
	Game* m_game;
	ButtonState* m_keys;
	ButtonState* m_mouse_buttons;
};
