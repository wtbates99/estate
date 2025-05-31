# Weapons System

This directory contains the weapon system implementation, organized similar to the enemies directory structure.

## Directory Structure

```
weapons/
├── README.md                   # This file
├── melee_weapons.h             # Melee weapon class declarations
├── melee_weapons.cpp           # Melee weapon implementations
├── ranged_weapons.h            # Ranged weapon class declarations
├── ranged_weapons.cpp          # Ranged weapon implementations
├── weapon_factory.h            # Weapon factory declarations
└── weapon_factory.cpp          # Weapon factory implementations
```

## Weapon Categories

### Melee Weapons
- **Sword**: High damage, medium cooldown, medium range
  - Damage: 25, Cooldown: 0.8s, Range: 60px
  - Asset: `assets/weapons/sword.png`
- **Dagger**: Low damage, fast cooldown, short range
  - Damage: 15, Cooldown: 0.5s, Range: 45px
  - Asset: `assets/weapons/dagger.png`

### Ranged Weapons
- **Bow**: Medium damage, medium cooldown, long range
  - Damage: 20, Cooldown: 1.0s, Range: 250px
  - Asset: `assets/weapons/bow.png`
- **Crossbow**: High damage, slow cooldown, very long range
  - Damage: 35, Cooldown: 1.5s, Range: 300px
  - Asset: `assets/weapons/crossbow.png`

## Asset System

Each weapon loads its visual representation from PNG files located in `assets/weapons/`. The assets are automatically loaded when the weapon is created and are positioned relative to the player during combat.

### Asset Features
- Automatic loading with fallback to basic shapes if PNG not found
- Dynamic positioning relative to player
- Animation support (scaling, rotation) during attacks
- Transparent backgrounds for proper layering

## Factory System

The weapon factory provides several creation methods:
- `createRandomWeapon()`: Creates a random weapon
- `createMeleeWeapon()`: Creates a random melee weapon
- `createRangedWeapon()`: Creates a random ranged weapon
- `createSword()`, `createDagger()`, `createBow()`, `createCrossbow()`: Create specific weapons

## Usage

```cpp
#include "weapons/weapon_factory.h"

// Create a specific weapon
auto sword = createSword();

// Create a random weapon
auto randomWeapon = createRandomWeapon();

// Create by category
auto meleeWeapon = createMeleeWeapon();
auto rangedWeapon = createRangedWeapon();
```

## Integration

To use the new weapon system in your code:

1. Include the appropriate headers from the `weapons/` directory
2. Use the weapon factory to create weapons
3. Ensure the `assets/weapons/` directory contains the required PNG files
4. The weapons will automatically handle asset loading and rendering 