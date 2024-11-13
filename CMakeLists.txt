# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(MyProject VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include the src directory for header files
include_directories(src)

# Add the executable by compiling all .cpp files in src
file(GLOB SOURCES "src/*.cpp")
add_executable(MyProject ${SOURCES})