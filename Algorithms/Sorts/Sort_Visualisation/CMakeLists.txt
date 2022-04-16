# Shoutout to CMake guide: https://eliasdaler.github.io/using-cmake/
cmake_minimum_required(VERSION 3.1)

IF (WIN32)
    set(CMAKE_C_COMPILER C:/msys64/mingw64/bin/gcc.exe)
    set(CMAKE_CXX_COMPILER C:/msys64/mingw64/bin/g++.exe)
    # this makes sure that .dll-files are build into build/ folder
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
ELSEIF (APPLE)
    # run 'where clang' and 'where clang++' to get paths
    set(CMAKE_C_COMPILER /usr/bin/clang)
    set(CMAKE_CXX_COMPILER /usr/bin/clang++)
ELSEIF (UNIX AND NOT APPLE)
    # run 'which gcc' and 'which g++' to get paths
    set(CMAKE_C_COMPILER /usr/bin/gcc)
    set(CMAKE_CXX_COMPILER /usr/bin/g++)
ENDIF()
    
# Get project name from folder name
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

# Use c++ 17
set(CMAKE_CXX_STANDARD 17)
# Enable additional warnings and debug mode
# https://caiorss.github.io/C-Cpp-Notes/compiler-flags-options.html
set (CMAKE_CXX_FLAGS "-g -W -pedantic")

# All .cpp-files
set(SOURCES src/main.cpp src/managers/application.cpp src/managers/column_manager.cpp src/managers/event_manager.cpp src/managers/window_manager.cpp src/managers/sort_manager.cpp src/sorts/bubblesort.cpp src/sorts/insertionsort.cpp src/sorts/quicksort.cpp src/sorts/radixsort.cpp src/sorts/selectionsort.cpp src/sorts/mergesort.cpp)
# folder with .h files, e.g. include/
include_directories(src)

# 3rd party dependencies
add_subdirectory(lib)

# Here the .c/.cpp files get linked
add_executable(${PROJECT_NAME} ${SOURCES})

# Here we link 3rd party library ImGui-SFML
# No need to link SFML implicitly as ImGui-SFML does it for you!
# Refer to https://eliasdaler.github.io/using-cmake/
target_link_libraries(${PROJECT_NAME} PRIVATE ImGui-SFML::ImGui-SFML)

# Additional resources like images and fonts
# We want the to copy from permanent folder "res" to temporary folder "build"
configure_file(res/computer-modern.ttf res/computer-modern.ttf COPYONLY)
configure_file(res/smile.png res/smile.png COPYONLY)

# Set "a" to be the output filename
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "a")