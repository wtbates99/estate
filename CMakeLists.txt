cmake_minimum_required(VERSION 3.10)
project(estate)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add source files
set(SOURCES
    src/main.cpp
    src/player.cpp
    src/enemy.cpp
    src/minimap.cpp
)

# Add header files
set(HEADERS
    src/player.h
    src/enemy.h
    src/minimap.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Copy resources to build directory
file(COPY ${CMAKE_SOURCE_DIR}/assets/ARIAL.TTF DESTINATION ${CMAKE_BINARY_DIR})
