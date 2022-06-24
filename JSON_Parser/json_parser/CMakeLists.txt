cmake_minimum_required(VERSION 3.5)

project(json_parser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
    src/jsonparser.cpp
    src/jsontree.cpp
    include/jsonparser.h
    include/jsontree.h
    include/symbols.h)

add_executable(json_parser ${SOURCES})
