#include "input.hpp"

InputManager::InputManager(Game* game) {
	this->m_game = game;
	m_keys = new ButtonState[SDL_NUM_SCANCODES];
	m_mouse_buttons = new ButtonState[8];
}

InputManager::~InputManager() {
	delete[] m_keys;
	delete[] m_mouse_buttons;
	m_keys = nullptr;
	m_mouse_buttons = nullptr;
	m_game = nullptr;
}

void InputManager::Update() {
	const uint8_t* keystate = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
		if (m_keys[i].pressed != keystate[i]) m_keys[i].on_edge = true;
		else m_keys[i].on_edge = false;
		m_keys[i].pressed = keystate[i];
	}
	uint8_t mouse_buttons = SDL_GetMouseState(nullptr, nullptr);
	for (int i = 0; i < 8; i++) {
		if (m_mouse_buttons[i].pressed != (mouse_buttons & SDL_BUTTON(i+1))) m_mouse_buttons[i].on_edge = true;
		else m_mouse_buttons[i].on_edge = false;
		m_mouse_buttons[i].pressed = mouse_buttons & SDL_BUTTON(i+1);
	}
}

ButtonState InputManager::QueryKey(int scancode) {return m_keys[scancode];}
ButtonState InputManager::QueryMouseButton(int index) {return m_mouse_buttons[index];}

bool InputManager::SomethingPressed() {
	for (size_t i = 0; i < SDL_NUM_SCANCODES; i++) {
		if (m_keys[i].pressed) return true;
	}
	for (size_t i = 0; i < 8; i++) {
		if (m_mouse_buttons[i].pressed) return true;
	}
	return false;
}

float InputManager::GetAxis(int scancode_n, int scancode_p) {
	return (QueryKey(scancode_p).pressed ? 1.0f : 0.0f) - (QueryKey(scancode_n).pressed ? 1.0f : 0.0f);
}
vec2 InputManager::GetVector(int scancode_nx, int scancode_px, int scancode_ny, int scancode_py) {
	return vec2(GetAxis(scancode_nx, scancode_px), GetAxis(scancode_ny, scancode_py));
}

ivec2 InputManager::GetMousePos() {
	int x,y;
	SDL_GetMouseState(&x,&y);
	return ivec2{x,y};
}

ivec2 InputManager::GetMouseDelta() {
	int x,y;
	SDL_GetRelativeMouseState(&x,&y);
	return ivec2{x,y};
}

void InputManager::SetMouseCaptured(bool captured) {
	SDL_SetRelativeMouseMode(captured ? SDL_TRUE : SDL_FALSE);
}
