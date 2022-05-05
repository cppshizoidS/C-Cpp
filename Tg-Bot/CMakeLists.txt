cmake_minimum_required(VERSION 3.10)
project(toadcraft VERSION 0.1.0 LANGUAGES CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(${CMAKE_SOURCE_DIR})
include_directories(include)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    bot
    src/main.cpp
    src/router.cpp
    src/command_handlers.cpp
)

target_link_libraries(bot ${CONAN_LIBS})

