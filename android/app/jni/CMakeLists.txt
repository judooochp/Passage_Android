cmake_minimum_required(VERSION 3.10)

project(GAME)

# SDL2 source is the git submodule in ./SDL (release-2.32.10).
add_subdirectory(SDL)

# The game itself: the repository root's CMakeLists.txt builds libmain.so.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../.. passage)
