cmake_minimum_required(VERSION 3.16.0)
project(map VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug)
endif()

option(BUILD_EXAMPLE "Build example code" OFF)
option(BUILD_WITH_COVERAGE "Build with coverage" OFF)
option(BUILD_PYTHON_BINDINGS "Build python bindings for scripting" OFF)

if(BUILD_WITH_COVERAGE)
    set(CMAKE_CXX_FLAGS "-g -O0 -Wall --coverage")
endif()

add_subdirectory(map)

if (BUILD_PYTHON_BINDINGS)
    add_subdirectory(scripts)
endif()

if(BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

enable_testing()
