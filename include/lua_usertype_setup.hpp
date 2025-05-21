#include <sol/sol.hpp>

class Game;
class GameObject;

void lua_usertype_setup(Game* game, std::shared_ptr<sol::state> lua_state, GameObject* game_object);
