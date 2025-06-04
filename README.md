# Estate - 2D Action Survival Game

Estate is a feature-rich 2D action survival game built with C++ and SFML. Fight enemies, collect weapons, develop talents, and survive in various environments with a comprehensive progression system.

## Features

- **Combat System**: Melee and ranged weapons with different combat mechanics
- **Enemy AI**: Multiple enemy types with different behaviors and combat patterns
- **Talent System**: Unlock and upgrade abilities as you progress
- **Multiple Environments**: Various background environments to explore
- **Health System**: Health packs and survival mechanics
- **Minimap**: Navigate the world with an integrated minimap
- **Asset Management**: Organized weapon, enemy, and environment textures

## Project Structure

```
estate/
├── assets/               # Game assets and resources
│   ├── backgrounds/      # Background textures
│   ├── textures/         # Game sprites and textures
│   │   └── enemies/      # Enemy sprite textures
│   ├── weapons/          # Weapon textures
│   └── ARIAL.TTF         # Font file
├── src/                  # Source code
│   ├── main.cpp          # Main game loop
│   ├── config.h          # Game configuration
│   ├── player.h/cpp      # Player character logic
│   ├── enemy.h/cpp       # Base enemy class
│   ├── weapon.h/cpp      # Weapon system
│   ├── talent.h/cpp      # Talent/skill system
│   ├── minimap.h/cpp     # Minimap functionality
│   ├── background.h/cpp  # Background rendering
│   ├── health_pack.h/cpp # Health item system
│   ├── backgrounds/      # Background system implementations
│   ├── enemies/          # Enemy type implementations
│   ├── weapons/          # Weapon type implementations
│   └── talents/          # Talent system implementations
├── build.sh              # Build script
├── CMakeLists.txt        # CMake configuration
└── README.md             # This file
```

## Setup

### macOS

#### Install Dependencies
```bash
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install SFML and CMake
brew install sfml cmake
```

#### Clone Repository
```bash
git clone https://github.com/wtbates99/estate.git
cd estate
```

### Linux (Ubuntu/Debian)

#### Install Dependencies
```bash
sudo apt-get update
sudo apt-get install g++ cmake libsfml-dev
```

#### Clone Repository
```bash
git clone https://github.com/wtbates99/estate.git
cd estate
```

### Windows (MinGW)

#### Install MSYS2 and Dependencies
```bash
pacman -Syu
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-sfml
```

#### Clone Repository
```bash
git clone https://github.com/wtbates99/estate.git
cd estate
```

## Build

### Quick Build (Recommended)
Use the provided build script:
```bash
chmod +x build.sh
./build.sh
```

### Manual Build
If you prefer to build manually:

```bash
mkdir -p build
cd build
```

#### macOS/Linux
```bash
cmake ..
cmake --build . -j$(nproc)
```

#### Windows
```bash
cmake .. -G "MinGW Makefiles"
cmake --build . -j$(nproc)
```

## Run

After building, run the game:

### macOS/Linux
```bash
./estate
```

### Windows
```bash
estate.exe
```

## Game Controls

- **WASD**: Move player
- **Mouse**: Aim and attack
- **B**: Return to background selection
- **ESC**: Exit game/Skip talent selection
- **Arrow Keys**: Navigate talent selection
- **Enter**: Select talent

## Development

### Adding New Content

1. **New Enemies**: Add implementations in `src/enemies/` and update the enemy factory
2. **New Weapons**: Add implementations in `src/weapons/` and update the weapon factory  
3. **New Talents**: Add implementations in `src/talents/` and update the talent factory
4. **New Backgrounds**: Add implementations in `src/backgrounds/` and update the background factory
5. **Assets**: Place new textures in appropriate `assets/` subdirectories

### Building in Debug Mode

The game includes debug features enabled by default:
- FPS counter
- Collision box visualization
- Debug console output

Configure these in `src/config.h`.

## Troubleshooting

### SFML not found during build
- **macOS**: Ensure SFML is installed via Homebrew: `brew install sfml`
- **Linux**: Ensure `libsfml-dev` is correctly installed
- **Windows**: Make sure SFML is properly installed via MSYS2

### Font loading errors
Make sure `assets/ARIAL.TTF` exists and is accessible. The build process should copy assets to the build directory automatically.

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is open source. Please add an appropriate license file for your preferred license terms.
