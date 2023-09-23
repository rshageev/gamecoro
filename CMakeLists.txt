cmake_minimum_required(VERSION 3.12)

project(GameCoro)

# Language standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(GameCoro INTERFACE)
target_include_directories(GameCoro INTERFACE gamecoro/)

add_subdirectory(tests)