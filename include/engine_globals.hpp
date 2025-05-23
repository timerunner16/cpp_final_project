#include <sol/sol.hpp>

class Game;
class GameObject;

void engine_globals(Game* game, std::shared_ptr<sol::state> lua_state, GameObject* game_object);
