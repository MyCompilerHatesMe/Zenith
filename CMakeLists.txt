# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Define the project
project(Zenith VERSION 0.1.0 LANGUAGES CXX)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directory for the final executable to be inside a 'bin' folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add the main executable target
add_executable(zenith
    interpreter/interpreter.cpp
    interpreter/lexer/lexer.cpp
    interpreter/parser/evaluator.cpp
)

#As your project grows, you can add more source files like this:
# target_sources(zenith PRIVATE
    # interpreter/lexer/lexer.cpp
    # interpreter/parser/parser.cpp
# )

target_include_directories(zenith PRIVATE ${CMAKE_SOURCE_DIR})

# Enable common warnings for better code quality
if(MSVC)
    target_compile_options(zenith PRIVATE /W4)
else()
    target_compile_options(zenith PRIVATE -Wall -Wextra -pedantic)
endif()