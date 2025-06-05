#include "win_usertype.hpp"
#include "window.hpp"

void win_usertype_setup(std::shared_ptr<sol::state> lua_state) {
	sol::usertype<Window> window_data_type = lua_state->new_usertype<Window>(
		"Window", sol::no_constructor,
		"Width", sol::readonly_property(&Window::GetWidth),
		"Height", sol::readonly_property(&Window::GetHeight),
		"Downscale", sol::readonly_property(&Window::GetDownscale),
		"Focused", sol::readonly_property(&Window::GetFocused),
		"SetWireframeEnabled", &Window::SetWireframeEnabled,
		"DrawString", &Window::DrawString
	);
}
