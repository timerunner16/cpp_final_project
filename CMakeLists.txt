cmake_minimum_required(VERSION 3.5)
project(cpp_final_project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
	cpp_final_project
	WIN32
	src/main.cxx
	src/game.cxx
	src/window.cxx
	src/workspace.cxx
	src/resource_manager.cxx
	src/game_object.cxx
	src/mesh.cxx
	src/shader.cxx
	src/camera.cxx
	src/gltexture.cxx
	src/material.cxx
	src/transform.cxx
)
target_include_directories(cpp_final_project PRIVATE include)


find_package(glm CONFIG REQUIRED)
target_link_libraries(cpp_final_project PRIVATE glm::glm)


find_package(Lua 5.4 EXACT REQUIRED)
target_link_libraries(cpp_final_project PRIVATE ${LUA_LIBRARIES})
target_include_directories(cpp_final_project PRIVATE ${LUA_INCLUDE_DIR})


find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
target_link_libraries(cpp_final_project PRIVATE SDL2::SDL2)
target_include_directories(cpp_final_project PRIVATE ${SDL2_INCLUDE_DIRS})


find_package(SDL2_image REQUIRED)
target_link_libraries(cpp_final_project PRIVATE SDL2_image::SDL2_image)
target_include_directories(cpp_final_project PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})


set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
target_link_libraries(cpp_final_project PRIVATE OpenGL::OpenGL)
target_include_directories(cpp_final_project PRIVATE ${OPENGL_INCLUDE_DIRS})


find_package(GLEW REQUIRED)
target_link_libraries(cpp_final_project PRIVATE GLEW::GLEW)
target_include_directories(cpp_final_project PRIVATE ${GLEW_INCLUDE_DIRECTORIES})


add_subdirectory(third_party)
target_link_libraries(cpp_final_project PRIVATE sol2::sol2)


add_custom_target(
	copy_assets
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_dependencies(cpp_final_project copy_assets)
