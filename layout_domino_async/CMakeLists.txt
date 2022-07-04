cmake_minimum_required(VERSION 3.16.3)
project(layout_domino)


set(CMAKE_CXX_STANDARD 17)
add_executable(layout_domino main.cpp Field.cpp Field.h Runner.cpp Runner.h Template.cpp Template.h Coord.cpp Coord.h Cell.cpp Cell.h Tiletype.h FileWriter.h SimpleParser.h)

