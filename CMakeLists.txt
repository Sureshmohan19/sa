# Set the minimum required version of CMake and define the project.
cmake_minimum_required(VERSION 3.10)
project(sa_compiler C CXX)

# Set the C++ standard to a modern version (C++17 or higher is good).
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# LLVM Configuration
# Point to the lib/cmake/llvm directory where LLVMConfig.cmake is located
set(LLVM_DIR "/opt/homebrew/opt/llvm/lib/cmake/llvm")

# Find LLVM and explicitly request the components we need.
find_package(LLVM REQUIRED CONFIG)

# Print LLVM package for confirmation
message(STATUS "Found LLVM ${LLVM_VERSION}")

# Include LLVM directories
include_directories(${LLVM_INCLUDE_DIRS})

# Include LLVM definitions
add_definitions(${LLVM_DEFINITIONS})

# --- THE DEFINITIVE FIX ---
# Manually get the library names for the components we need.
# This will populate the SA_LLVM_LIBS variable.
llvm_map_components_to_libnames(SA_LLVM_LIBS core support irreader)
# --- END FIX PART 1 ---

# Add the 'src' directory to the include path.
include_directories(src)

# Create our executable, named 'sac' (sa compiler).
add_executable(sac
    src/main.cpp
    src/core/lib/Token.cpp
    src/frontend/lib/Lexer.cpp
    src/frontend/lib/Parser.cpp
    src/ast/lib/Visitor.cpp
    src/backend/lib/CodeGen.cpp
)

# CMake will rebuild if any changes in header files
target_sources(sac PRIVATE
    src/core/include/Token.h
    src/frontend/include/Lexer.h
    src/frontend/include/Parser.h
    src/ast/include/Decl.h
    src/ast/include/Expr.h
    src/ast/include/Stmt.h
    src/ast/include/Visitor.h
    src/backend/include/CodeGen.h
)

# --- THE DEFINITIVE FIX ---
# Link against the list of libraries we just populated.
target_link_libraries(sac PRIVATE ${SA_LLVM_LIBS})
# --- END FIX PART 2 ---

# A small convenience to print the build type during configuration.
message(STATUS "Configuring sa_compiler...")