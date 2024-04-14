cmake_minimum_required(VERSION 3.28)
project(DoomLike)

set(CMAKE_CXX_STANDARD 26)

include_directories(/opt/homebrew/include/SDL2)


link_directories(/opt/homebrew/lib)
link_libraries(SDL2)

add_executable(DoomLike main.cpp)
