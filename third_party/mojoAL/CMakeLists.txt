cmake_minimum_required(VERSION 3.12)
project(mojoAL)

find_package(SDL2 REQUIRED)

add_library(mojoAL STATIC mojoal.c)
target_include_directories(mojoAL PUBLIC ${SDL2_INCLUDE_DIRS} AL)
target_link_libraries(mojoAL ${SDL2_LIBRARIES})
