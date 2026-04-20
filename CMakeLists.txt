cmake_minimum_required(VERSION 3.15)
project(world LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL3 REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/World.cpp
)

target_link_libraries(${PROJECT_NAME} sdl::sdl)
