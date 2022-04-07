cmake_minimum_required(VERSION 3.10)

find_package(GTest REQUIRED)
find_package(SqliteOrm REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

if (DEBUG)
    add_compile_options(-g3 -ggdb)
endif()


set(EXEC_ACCOUNT account_test)
set(SRC_ACCOUNT account.cpp
    ../src/account.cpp
)

set(EXEC_SERVER server_test)
set(SRC_SERVER server.cpp)

add_executable(${EXEC_ACCOUNT} ${SRC_ACCOUNT})
target_link_libraries(${EXEC_ACCOUNT}
    ${GTEST_LIBRARIES}
    sqlite_orm::sqlite_orm
    pthread
)

add_executable(${EXEC_SERVER} ${SRC_SERVER})
target_link_libraries(${EXEC_SERVER}
    ${CONAN_LIBS}
    pthread
)

