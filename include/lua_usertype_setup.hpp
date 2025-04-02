#include <sol/sol.hpp>

class Game;
class GameObject;

void lua_usertype_setup(Game* game, sol::state& lua_state, GameObject* game_object);
