if (DEBUG)
    add_compile_options(-g3 -ggdb)
endif()

add_executable(test main.cpp)
target_link_libraries(test ${CONAN_LIBS})

