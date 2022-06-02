cmake_minimum_required(VERSION 3.22)
project(Pac_Man)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Pac_Man
        Animation.cpp
        Animation.h
        Console.cpp
        Console.h
        Directions.h
        Engine.cpp
        Engine.h
        Entity.cpp
        Entity.h
        GameState.cpp
        GameState.h
        Ghost.cpp
        Ghost.h
        Labyrinth.cpp
        Labyrinth.h
        Main.cpp
        PacMan.cpp
        PacMan.h
        PlayingState.cpp
        PlayingState.h
        Resources.cpp
        Resources.h
        StateManager.cpp
        StateManager.h)
