set(BIN ads-l1-v5)
set(SRC main.cpp)

add_executable(${BIN} ${SRC})
target_link_libraries(${BIN} ${CONAN_LIBS} validators)

