# Set the minimum required version of CMake and define the project.
cmake_minimum_required(VERSION 3.10)
project(sa_compiler CXX)

# Set the C++ standard to a modern version (C++17 or higher is good).
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add the 'src' directory to the include path.
# This lets us write #include "core/Token.h" from any file.
include_directories(src)

# Create our executable, named 'sac' (sa compiler).
# List all the .cpp source files needed to build it.
add_executable(sac
    src/main.cpp
    src/core/Token.cpp
    src/frontend/Lexer.cpp
)

# A small convenience to print the build type during configuration.
message(STATUS "Configuring sa_compiler...")