# CMake Properties
cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Project Properties
project(
    app-t
    VERSION 0.1
    DESCRIPTION "Base C/C++ CMake application"
    LANGUAGES CXX 
)

# Project Library
include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_library(app-t-lib ${SOURCES})

# Project Main
add_subdirectory(app)
