cmake_minimum_required(VERSION 3.27)
project(DoomLike)

set(CMAKE_CXX_STANDARD 26)

include_directories(/opt/homebrew/include/)

link_directories(/opt/homebrew/lib)
link_libraries(SDL2)
link_libraries(SDL2_gfx)
link_libraries(SDL2_image)
link_libraries(SDL2_ttf)

add_executable(DoomLike main.cpp
        src/Player.cpp
        include/Player.h
        include/Map.h
        include/constante.h
        src/Monster.cpp
        include/Monster.h
        src/constante.cpp
        src/Textures.cpp
        include/Textures.h
        src/Game.cpp
        include/Game.h
        src/Menu.cpp
        include/Menu.h
        src/MultiPlayer.cpp
        include/MultiPlayer.h
)
