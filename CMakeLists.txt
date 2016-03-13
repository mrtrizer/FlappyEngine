cmake_minimum_required(VERSION 2.8)
project (FlappyEngine)

add_compile_options("-std=c++1y")

file(GLOB SOURCES src/*.cpp src/core/*.cpp )

find_package(GLEW)
find_package(GLUT)
find_package(PNG)
find_package(SDL_image)

if (GLEW_FOUND)
    file(GLOB SOURCES_TMP src/gl/*.cpp )
    set(SOURCES ${SOURCES} ${SOURCES_TMP})
endif()

if (GLUT_FOUND)
    file(GLOB SOURCES_TMP src/glut/*.cpp)
    set(SOURCES ${SOURCES} ${SOURCES_TMP})
endif()

if (PNG_FOUND)
    file(GLOB SOURCES_TMP src/png/*.cpp)
    set(SOURCES ${SOURCES} ${SOURCES_TMP})
endif()

if (SDL_IMAGE_FOUND)
    file(GLOB SOURCES_TMP src/sdl/*.cpp)
    set(SOURCES ${SOURCES} ${SOURCES_TMP})
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (GLEW_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${GLEW_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
endif()

if (GLUT_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${GLUT_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${GLUT_LIBRARY})
endif()

if (PNG_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${PNG_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${PNG_LIBRARY})
endif()

if (SDL_IMAGE_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${SDL_IMAGE_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${SDL_IMAGE_LIBRARIES})
endif()

target_include_directories(${PROJECT_NAME} PUBLIC src/)
