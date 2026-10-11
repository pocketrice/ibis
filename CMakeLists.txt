cmake_minimum_required(VERSION 3.30)
project(ibis)

set(CMAKE_CXX_STANDARD 20)

include_directories(ext/glad/include)
include_directories(ext/soil/include)
find_package(glfw3 3.4 REQUIRED)
find_library(corefoundation CoreFoundation)
find_library(opengl OpenGL)
find_library(openal, openal-soft)
find_library(glm, GLM)

add_library(glad STATIC
    ext/glad/src/gl.c
)

add_executable(ibis src/main.cpp)
target_link_libraries(ibis PRIVATE glfw glad /usr/local/lib/libSOIL.a ${glm} ${opengl} ${openal} ${corefoundation} )
