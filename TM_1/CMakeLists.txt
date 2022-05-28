cmake_minimum_required(VERSION 3.21)
project(tm1)

set(CMAKE_CXX_STANDARD 14)

add_executable(tm1 main.cpp table_const.h table_var.h lexeme.h translator.h)
