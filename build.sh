#!/bin/bash

# Create build directory if it doesn't exist
mkdir -p build

# Navigate to build directory
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build . -j$(nproc)

# Copy resources to build directory
cp ../assets/ARIAL.TTF .

# Run the game
./estate 