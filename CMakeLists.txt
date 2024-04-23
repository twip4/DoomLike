cmake_minimum_required(VERSION 3.28)
project(DoomLike)

set(CMAKE_CXX_STANDARD 26)

include_directories(/opt/homebrew/include/)

link_directories(/opt/homebrew/lib)
link_libraries(SDL2)
link_libraries(SDL2_gfx)

add_executable(DoomLike main.cpp
        src/Player.cpp
        include/Player.h
        include/Map.h
        include/constante.h)
