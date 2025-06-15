set(SOL2_LUA_VERSION "5.4" CACHE STRING "The version of Lua used")
set(SOL2_BUILD_LUA FALSE CACHE BOOL "Always build Lua, do not search for it in the system")
add_subdirectory(sol2)

set(GLM_BUILD_TESTS OFF)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(glm)

add_subdirectory(TrianglePP)

add_subdirectory(mojoAL)
