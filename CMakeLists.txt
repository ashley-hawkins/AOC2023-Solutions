cmake_minimum_required(VERSION 3.26)
project(AOC2023 CXX)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Werror -Wall -Wextra -pedantic)

add_subdirectory(01)
add_subdirectory(02)
add_subdirectory(03)
