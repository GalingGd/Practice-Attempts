cmake_minimum_required(VERSION 3.21)

project(PracticeAttempts VERSION 1.0.0)

find_package(Geode REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_geode_mod(PracticeAttempts
    SOURCES ${SOURCES}
    RESOURCES logo.png
)
