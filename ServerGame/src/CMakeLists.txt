cmake_minimum_required(VERSION 3.10)

find_package(SqliteOrm REQUIRED)

if (DEBUG)
    add_compile_options(-g3 -ggdb)
endif()


set(EXEC tmp)
set(SRC
    tmp.cpp
    account.cpp
)

add_executable(${EXEC} ${SRC})
target_link_libraries(${EXEC}
    sqlite_orm::sqlite_orm
    ${CONAN_LIBS}
    pthread
)

