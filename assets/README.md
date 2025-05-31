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