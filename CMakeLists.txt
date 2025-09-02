cmake_minimum_required(VERSION 3.10)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(black_hole)
# cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_PREFIX_PATH cmake/glfw3)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC includes)


add_executable(black_hole src/main.cpp src/glad.c src/shader.cpp)

target_link_libraries(black_hole glfw OpenGL::GL glad)
