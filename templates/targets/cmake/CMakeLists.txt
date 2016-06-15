cmake_minimum_required(VERSION 2.8)

project({!name!})

add_custom_target(PreBuild ALL
    COMMAND flappy prebuild cmake
)

set(CMAKE_BUILD_TYPE Debug)

if (NOT ${CMAKE_CURRENT_LIST_DIR} MATCHES ${CMAKE_BINARY_DIR})
    add_custom_target(PreBuildResCp ALL
        COMMAND cp ${CMAKE_CURRENT_LIST_DIR}/res ${CMAKE_BINARY_DIR} -R
    )
endif()

add_definitions("-std=c++1y")
include_directories(
    "./src/"
    "../../src/"
{?printList(inc_dirs,"\t\t\"../../*\"\n")?}
)
add_subdirectory(../../{!engine!} engine)

file(GLOB SOURCES src/*.cpp ./*.cpp)

{?printList(fileList(projectDir + "/src/",".cpp"),"list(APPEND SOURCES ../../src/*)\n",exclude)?}

set(EXECUTABLE_NAME {!cmake.exec_name!})

add_executable(${EXECUTABLE_NAME} ${SOURCES})

target_link_libraries(${EXECUTABLE_NAME} FlappyEngine)

find_package(PNG)
find_package(SDL_image)

if (PNG_FOUND)
    add_definitions(-DPNG_FOUND)
endif()

if (SDL_IMAGE_FOUND)
    add_definitions(-DSDL_IMAGE_FOUND)
endif()
