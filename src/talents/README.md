# Talent System

This directory contains the talent system implementation, organized similar to the weapons and enemies directory structure.

## Directory Structure

```
talents/
├── README.md                   # This file
├── combat_talents.h            # Combat talent class declarations
├── combat_talents.cpp          # Combat talent implementations
├── defensive_talents.h         # Defensive talent class declarations
├── defensive_talents.cpp       # Defensive talent implementations
├── utility_talents.h           # Utility talent class declarations
├── utility_talents.cpp         # Utility talent implementations
├── talent_factory.h            # Talent factory declarations
└── talent_factory.cpp          # Talent factory implementations
```

## Overview

The talent system provides a randomly generated talent tree that appears when the player levels up, pausing the game until a talent is selected. This creates a roguelike progression system where each playthrough has different advancement options.

## Key Features

### Randomly Generated Trees
- Each level up generates a new random talent tree
- 5-8 talents per tree with dependency relationships
- Grid-based layout with some randomness for organic feel
- Prerequisites and unlocking patterns create strategic choices

### Talent Categories

#### Combat Talents
- **Damage Boost**: Increases weapon damage (Common)
- **Attack Speed**: Faster weapon attacks (Common)  
- **Critical Strike**: Chance for critical hits (Uncommon)
- **Vampirism**: Life steal on damage dealt (Rare)
- **Berserk**: Increased damage at low health (Epic)

#### Defensive Talents
- **Health Boost**: Increases maximum health (Common)
- **Armor**: Reduces incoming damage (Uncommon)
- **Regeneration**: Heals over time (Rare)
- **Shield**: Energy shield that absorbs damage (Epic)
- **Dodge**: Chance to avoid damage completely (Rare)

#### Utility Talents
- **Speed Boost**: Increases movement speed (Common)
- **Experience Boost**: Gain more experience (Uncommon)
- **Gold Find**: Find more gold from enemies (Common)
- **Lucky Drops**: Better item drop chances (Rare)
- **Minimap Mastery**: Improved minimap vision (Uncommon)

### Rarity System
- **Common** (White): Basic improvements, low impact
- **Uncommon** (Green): Moderate improvements 
- **Rare** (Blue): Significant improvements
- **Epic** (Purple): Major improvements
- **Legendary** (Yellow): Game-changing improvements

### Leveling and Upgrades
- Each talent can be leveled up multiple times (max level 3)
- Higher levels provide stronger effects
- Talents can evolve into upgraded versions with different names
- Some talents unlock new mechanics (critical strikes, shields, etc.)

## Game Integration

### Level Up Flow
1. Player gains enough experience to level up
2. Game pauses and switches to `TALENT_SELECTION` state
3. Random talent tree is generated using `TalentTree::generateRandomTree()`
4. Player navigates with arrow keys and selects with Enter
5. Selected talent is applied to player stats/mechanics
6. Game resumes normal play

### UI Controls
- **Left/Right Arrows**: Navigate available talents
- **Enter**: Select highlighted talent
- **Escape**: Skip talent selection (debug feature)

### Visual Elements
- Talent boxes with rarity-colored borders
- Connecting lines showing dependencies
- Selected talents highlighted in green
- Available talents highlighted when selectable
- Semi-transparent overlay pauses game visually

## Factory Pattern

The talent system follows the same factory pattern as weapons and enemies:

```cpp
#include "talents/talent_factory.h"

// Create random talent
auto talent = createRandomTalent();

// Create by category
auto combatTalent = createCombatTalent();
auto defensiveTalent = createDefensiveTalent();
auto utilityTalent = createUtilityTalent();

// Create specific talent
auto damageBoost = createDamageBoostTalent();
```

## Implementation Details

### Base Talent Class
```cpp
class Talent {
public:
    virtual void apply(Player& player) = 0;
    virtual void onLevelUp(Player& player) = 0;
    virtual std::unique_ptr<Talent> createUpgrade() const = 0;
    
    // Getters for name, description, category, rarity, level
    // UI drawing methods
};
```

### Talent Tree Structure
```cpp
struct TalentNode {
    std::unique_ptr<Talent> talent;
    std::vector<int> prerequisites;  // Required talents
    std::vector<int> unlocks;        // Talents this enables
    bool isUnlocked;
    bool isSelected;
    sf::Vector2f position;          // UI position
};
```

### Player Integration
The player class includes:
- `TalentTree talentTree_` - Current talent tree
- `int pendingLevelUps_` - Number of level ups waiting
- `bool needsLevelUp()` - Check if talent selection needed
- `void processLevelUp()` - Generate new talent tree
- `void selectTalent(int index)` - Apply selected talent

## Expansion Ideas

### Additional Talent Categories
- **Weapon Mastery**: Specific weapon type bonuses
- **Elemental**: Fire/ice/lightning damage types
- **Summoning**: Companion creatures or turrets
- **Passive Auras**: Buffs that affect nearby area

### Advanced Features
- **Talent Synergies**: Bonuses for taking related talents
- **Negative Talents**: High-risk, high-reward options
- **Branching Paths**: Mutually exclusive talent lines
- **Prestige System**: Carry some talents between runs

### UI Improvements
- **Search/Filter**: Find specific talent types
- **Tooltip Details**: Detailed stat information
- **Preview Mode**: See talent effects before selection
- **History**: Track talents taken in current run

## Testing

Use the debug key `X` to gain experience quickly and test the talent system:

```cpp
// In main game loop (debug only)
if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
    main_player->gainExperience(10);
}
```

This allows rapid testing of:
- Talent tree generation
- UI navigation and selection
- Talent effect application
- Game state transitions

## Performance Considerations

- Talent trees are generated on-demand (only when leveling)
- Trees are cleared after selection to save memory
- Factory pattern ensures efficient talent creation
- UI drawing is optimized for 60fps even with complex trees

The talent system provides endless replayability through randomized progression trees while maintaining the organized, scalable code structure established by the weapons and enemies systems. 