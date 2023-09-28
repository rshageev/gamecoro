cmake_minimum_required(VERSION 3.15)

project(GameCoro)

add_library(GameCoro STATIC
    src/coroutine.cpp
    src/updater.cpp
)
target_compile_features(GameCoro PUBLIC cxx_std_20)
target_include_directories(GameCoro PRIVATE include/gamecoro)
target_include_directories(GameCoro PUBLIC include)

add_subdirectory(tests)