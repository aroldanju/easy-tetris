cmake_minimum_required(VERSION 3.12)
project(easy-tetris)

set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCES src/*.cpp)

add_executable(easy-tetris ${SOURCES})
target_link_libraries(easy-tetris -lSDL2 -lSDL2_ttf)
# __DEV__
# __DEBUG__
target_compile_definitions(easy-tetris PRIVATE)