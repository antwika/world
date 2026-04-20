cmake_minimum_required(VERSION 3.15)
project(world LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GTest REQUIRED)
find_package(SDL3 REQUIRED)

enable_testing()

add_executable(world
    src/main.cpp
    src/World.cpp
)

target_link_libraries(world PRIVATE sdl::sdl)

add_executable(WorldTest
    tests/WorldTest.cpp
    src/World.cpp
)

target_include_directories(WorldTest PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(WorldTest PRIVATE
    gtest::gtest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(WorldTest)
