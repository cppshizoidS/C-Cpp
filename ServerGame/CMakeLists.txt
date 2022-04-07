cmake_minimum_required(VERSION 3.10)
project(tmp VERSION "0.1.0")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

if (TESTS)
    add_subdirectory(test)
endif()

