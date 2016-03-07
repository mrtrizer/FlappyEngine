cmake_minimum_required(VERSION 2.8)
project (FlappyEngine)
 
file(GLOB SOURCES src/*)
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC src/)
