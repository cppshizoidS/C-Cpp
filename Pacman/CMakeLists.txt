cmake_minimum_required(VERSION 3.22)
project(Pacman CXX)

set(CMAKE_CXX_STANDARD 11)
find_package(GLUT 3.7)

add_executable(Pacman main.cpp)
target_link_libraries(Pacman -lOpenGL32 -lfreeGLUT)
