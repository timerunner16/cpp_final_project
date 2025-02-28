cmake_minimum_required(VERSION 3.5)
project(cpp_final_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
	cpp_final_project
	WIN32
	src/main.cxx
	src/game.cxx
	src/window.cxx
	src/workspace.cxx
	src/game_object.cxx
)
target_include_directories(cpp_final_project PRIVATE include)


find_package(glm CONFIG REQUIRED)
target_link_libraries(cpp_final_project PRIVATE glm::glm)


find_package(Lua 5.3 EXACT REQUIRED)
target_link_libraries(cpp_final_project PRIVATE ${LUA_LIBRARIES})
target_include_directories(cpp_final_project PRIVATE ${LUA_INCLUDE_DIR})


find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
target_link_libraries(cpp_final_project PRIVATE SDL2::SDL2)
target_include_directories(cpp_final_project PRIVATE ${SDL2_INCLUDE_DIRS})


set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
target_link_libraries(cpp_final_project PRIVATE OpenGL::OpenGL)
target_include_directories(cpp_final_project PRIVATE ${OPENGL_INCLUDE_DIRS})


find_package(GLEW REQUIRED)
target_link_libraries(cpp_final_project PRIVATE GLEW::GLEW)
target_include_directories(cpp_final_project PRIVATE ${GLEW_INCLUDE_DIRECTORIES})


add_custom_command(
	TARGET cpp_final_project POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_SOURCE_DIR}/assets/
			$<TARGET_FILE_DIR:cpp_final_project>/assets/
)
