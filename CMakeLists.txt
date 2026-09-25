cmake_minimum_required(VERSION 3.30)
project(ibis)

set(CMAKE_CXX_STANDARD 20)

include_directories(ext/glad/include)
find_package(glfw3 3.4 REQUIRED)

add_executable(ibis src/main.cpp ext/glad/src/gl.c)
target_link_libraries(ibis PRIVATE glfw)
