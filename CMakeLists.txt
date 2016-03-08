cmake_minimum_required(VERSION 2.8)
project (FlappyEngine)

add_compile_options("-std=c++1y")

file(GLOB SOURCES src/*.cpp src/core/*.cpp src/gl/*.cpp src/glut/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC src/)
