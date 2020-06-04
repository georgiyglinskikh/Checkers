cmake_minimum_required(VERSION 3.16)
project(Checkers C)

set(CMAKE_C_STANDARD 11)

include_directories("C:/Program Files/gcc/include")

add_executable(Checkers main.c Game.c Game.h Checkers.c Checkers.h)

target_link_libraries(
        ${PROJECT_NAME}
        csfml-graphics
        csfml-window
        csfml-system
)