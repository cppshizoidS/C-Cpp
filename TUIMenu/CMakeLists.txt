cmake_minimum_required(VERSION 3.10)
project(tui-menu VERSION 0.3.0)

include_directories(${CMAKE_CURRENT_DIR} include)

include(FetchContent)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if (DEBUG)
    add_compile_options(-g3 -ggdb)
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BIN menu)
set(SRC main.cpp)

if (DOCS)
    add_subdirectory(docs)
endif()

if (ADS_L1_V3)
    add_subdirectory(examples/ads-l1-v3)
endif()

if (ADS_L1_V5)
    add_subdirectory(examples/ads-l1-v5)
endif()

add_library(validators STATIC src/tui-menu/validators.cpp)

add_executable(${BIN} ${SRC})
target_link_libraries(${BIN} ${CONAN_LIBS} validators)

