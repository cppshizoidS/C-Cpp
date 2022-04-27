cmake_minimum_required(VERSION 3.10)
project(lab2)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(include)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (TESTS)
    add_subdirectory(test)
endif()

if (DOCS)
    add_subdirectory(docs)
endif()

