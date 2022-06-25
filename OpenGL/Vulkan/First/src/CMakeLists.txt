cmake_minimum_required(VERSION 3.22)
project(Vulkan_Projects)

set(CMAKE_CXX_STANDARD 14)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

target_link_libraries(Vulkan_Projects GLFW)
find_package(OpenGL 4.6)
target_link_libraries(Vulkan_Projects OpenGL::GL)
add_executable(Vulkan_Projects main.cpp)
