cmake_minimum_required(VERSION 3.22)
project(src)

set(CMAKE_CXX_STANDARD 17)

add_executable(src main.cpp Node.h Pathfinding.cpp Pathfinding.hpp PublicVariables.h)