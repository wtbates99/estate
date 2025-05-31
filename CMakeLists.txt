cmake_minimum_required(VERSION 3.10)
project(estate LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML (adjust path if SFML is installed in a custom location)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add executable
add_executable(estate src/main.cpp)

# Link SFML libraries
target_link_libraries(estate PRIVATE sfml-graphics sfml-window sfml-system)
