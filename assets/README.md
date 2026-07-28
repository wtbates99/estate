# Game Assets

This directory contains all the game assets for the Estate game.

## Textures

### Enemy Textures (`textures/enemies/`)

Each enemy type has its own sprite texture:

- `grunt.png` - Basic red enemy, medium health and damage
- `scout.png` - Fast yellow enemy, low health but quick attacks
- `brute.png` - Large brown enemy, high health and charge attacks
- `assassin.png` - Magenta stealth enemy, can become semi-transparent
- `sniper.png` - Cyan long-range enemy, high damage but slow attacks
- `swarm.png` - Small orange enemies that move in orbit patterns

### Texture Requirements

- **Format**: PNG with transparency support
- **Recommended Size**: 64x64 pixels (will be scaled by the game)
- **Background**: Transparent
- **Style**: Should be clearly distinguishable from each other

### Scaling Information

The game applies different scales to each enemy type:
- Scout: 0.6x scale (smallest)
- Swarm: 0.5x scale (very small)
- Assassin: 0.7x scale (small-medium)
- Grunt: 0.8x scale (medium)
- Sniper: 0.9x scale (medium-large)
- Brute: 1.2x scale (largest)

## Replacing Placeholder Textures

The current textures are colorful placeholders with text labels. To replace them with proper artwork:

1. Create or obtain 64x64 PNG images for each enemy type
2. Name them according to the filenames listed above
3. Place them in the `textures/enemies/` directory
4. The game will automatically load them on next run

## Fallback System

If a texture file is missing or fails to load, the game will automatically fall back to colored rectangles matching the original design colors. 

## 🎯 Key Features

### **Multi-Background System**
- **7 Background Options**: Forest, Desert, Ocean, Mountains, City, Space, and Custom
- **Interactive Selection Menu**: Navigate with arrow keys, select with Enter
- **Factory Pattern**: Easily extensible for adding new backgrounds
- **Real-time Switching**: Press 'B' during gameplay to change backgrounds

### **Professional Architecture**
- **Similar to Enemies/Weapons**: Follows the same factory pattern you're already using
- **Smart Pointers**: Proper memory management
- **Fallback System**: Game continues even if backgrounds fail to load
- **Error Handling**: Graceful degradation with console logging

### **User Experience**
- **Visual Menu**: Descriptions for each background option
- **Flexible Controls**: Enter to select, Escape for random
- **Easy Setup**: Just drop PNG files in the backgrounds directory

## 📁 File Structure Created

```
src/
├── background_menu.h/.cpp          # Selection menu system
└── backgrounds/
    ├── background_types.h/.cpp     # Background definitions
    └── background_factory.h/.cpp   # Factory for creating backgrounds

assets/
└── backgrounds/                    # Where you place your images
    └── README.md                   # Instructions for users
```

## 🎮 How to Use

1. **Place your 5000x5000 background images** in `assets/backgrounds/` with names like:
   - `forest.png`, `desert.png`, `ocean.png`, etc.
   - Your custom background goes in `assets/background.png`

2. **Build and run**:
   ```bash
   ./build.sh
   ./build/estate
   ```

3. **Select your background** from the menu that appears when you start the game

4. **Switch backgrounds** anytime during gameplay by pressing 'B'

## 🔧 Easy to Extend

Adding new backgrounds is simple:
1. Add the background type to the enum
2. Add the background info to the list
3. Drop the image file in the backgrounds directory

The system is fully integrated with your existing game architecture and maintains the same coding patterns as your enemies and weapons systems! 