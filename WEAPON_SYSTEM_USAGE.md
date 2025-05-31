# Weapon System Usage Guide

## Overview

Your player now has a comprehensive weapon system with both melee and ranged weapons that automatically target the closest enemy!

## Controls

### Movement
- **WASD** or **Arrow Keys**: Move the player

### Combat
- **Space**: Manual attack (attacks closest enemy in range)
- **Auto-attack**: Enabled by default - automatically attacks when enemies are in range

### Weapon Switching
- **1**: Sword (Melee) - High damage, short range
- **2**: Bow (Ranged) - Medium damage, long range
- **3**: Dagger (Melee) - Fast attacks, short range  
- **4**: Crossbow (Ranged) - High damage, slow attacks, longest range

## Weapon Types

### Melee Weapons
- **Sword**: 25 damage, 60 range, 0.8s cooldown
- **Dagger**: 15 damage, 45 range, 0.5s cooldown
- Shows a white swing arc animation when attacking
- Deals damage instantly to the closest enemy

### Ranged Weapons
- **Bow**: 20 damage, 250 range, 1.0s cooldown, 350 projectile speed
- **Crossbow**: 35 damage, 300 range, 1.5s cooldown, 500 projectile speed
- Fires yellow projectiles that travel toward enemies
- Projectiles deal damage on contact

## Features

### Auto-Targeting
- All weapons automatically target the closest enemy within range
- No need to aim manually - just get in range and attack!

### Visual Feedback
- Current weapon info displayed above the player
- Melee weapons show swing animations
- Ranged weapons show projectile trails
- Health display shows current HP

### Smart Combat
- Weapons have cooldowns to balance gameplay
- Ranged projectiles have realistic physics
- Different weapons for different strategies

## Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

✅ **Build Status**: Successfully compiled! The executable `estate` is ready to run.

## Running the Game

```bash
# From the build directory
./estate

# Or from the project root
./build/estate
```

## Strategy Tips

1. **Use ranged weapons** to keep distance from tough enemies
2. **Switch to melee** for quick, high-damage kills when surrounded
3. **Dagger** is great for hit-and-run tactics
4. **Crossbow** is perfect for taking out dangerous enemies from afar
5. **Auto-attack** means you can focus on positioning and weapon switching

## Technical Details

The weapon system includes:
- **Automatic targeting**: Finds closest enemy within weapon range
- **Collision detection**: Projectiles accurately hit enemies
- **Visual effects**: Swing arcs and projectile animations
- **Performance optimized**: Efficient enemy searching and projectile management

Enjoy your new combat system! 