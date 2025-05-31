# SFML 2D Game

This is a basic 2D game project built with C++ and SFML, managed by CMake. It opens an 800x600 window, providing a solid foundation for your 2D game development.

## Setup

### Linux (Ubuntu/Debian)

#### Install Dependencies
```bash
sudo apt-get update
sudo apt-get install g++ cmake libsfml-dev
```

#### Clone Repository
```bash
git clone <repository-url>
cd piracte
```

### Windows (MinGW)

#### Install MSYS2 and Dependencies
```bash
pacman -Syu
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake
```

#### Install CMake
Download and install CMake from [cmake.org](https://cmake.org). Make sure to add it to your system's PATH during installation.

#### Download and Configure SFML
1. Download SFML 2.5.1 (MinGW 64-bit) from [sfml-dev.org](https://www.sfml-dev.org)
2. Extract the downloaded archive (e.g., `SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit.zip`) to a convenient location, for example, `C:\SFML-2.5.1`
3. Add `C:\SFML-2.5.1\bin` to your system's PATH environment variable

#### Clone Repository
```bash
git clone <repository-url>
cd piracte
```

## Build

After setting up, navigate to the project root and follow these steps to build:

```bash
mkdir build
cd build
```

### Linux
```bash
cmake -S .. -B . && cmake --build .
```

### Windows
```bash
cmake -S .. -B . -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=C:/SFML-2.5.1 && cmake --build .
```

## Run

Once built, you can run the game:

### Linux
```bash
./estate
```

### Windows
Before running, copy the necessary SFML DLLs (Dynamic Link Libraries) from `C:\SFML-2.5.1\bin` to your `build/` directory. You'll typically need:
- `sfml-graphics-2.dll`
- `sfml-window-2.dll`
- `sfml-system-2.dll`

Then, run the executable:
```bash
estate.exe
```

## Troubleshooting

### SFML not found during build
- **Linux**: Ensure `libsfml-dev` is correctly installed
- **Windows**: Double-check your SFML installation path and PATH environment variable. If issues persist, you might need to explicitly tell CMake where to find SFML by adding `-DSFML_DIR=C:/SFML-2.5.1/lib/cmake/SFML` to your cmake command

### DLL errors (Windows)
Make sure all required SFML DLLs are either in your `build/` directory or accessible via your system's PATH

### No window appears
Verify that `src/main.cpp` correctly uses the `sf::` namespace for SFML functions and objects (e.g., `sf::RenderWindow`, `sf::Event`)

## Next Steps

This project provides a minimal SFML setup. Here's how you can expand it:

1. **Add game logic**: Edit `src/main.cpp` to introduce sprites, handle user input, implement game states, and more!

2. **Include new source files**: If you create additional `.cpp` files, remember to update `CMakeLists.txt` to include them in your build. For example:

```cmake
add_executable(estate src/main.cpp src/newfile.cpp)
```
