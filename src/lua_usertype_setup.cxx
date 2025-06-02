#include "lua_usertype_setup.hpp"
#include "geom_usertype.hpp"
#include "go_usertype.hpp"
#include "prtcl_usertype.hpp"
#include "vdata_usertype.hpp"
#include "input_usertype.hpp"
#include "rsmgr_usertype.hpp"
#include "win_usertype.hpp"
#include "wkspc_usertype.hpp"
#include "engine_globals.hpp"
#include "game.hpp"

void lua_usertype_setup(Game* game, std::shared_ptr<sol::state> lua_state, GameObject* game_object) {
	geom_usertype_setup(lua_state, game_object);
	go_usertype_setup(lua_state, game_object);
	vdata_usertype_setup(lua_state, game_object);
	input_usertype_setup(lua_state, game_object);
	prtcl_usertype_setup(lua_state, game_object);
	rsmgr_usertype_setup(lua_state);
	win_usertype_setup(lua_state);
	wkspc_usertype_setup(lua_state);
	engine_globals(game, lua_state, game_object);
}
