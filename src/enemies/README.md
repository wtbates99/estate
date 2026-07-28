# Enemy System Organization Guide

## Overview

Your enemy system has been successfully refactored from a monolithic structure into a modular, organized system. This makes it much easier to maintain and scale as you add more enemy types.

## File Structure

```
src/
├── enemy.h              # Base Enemy class definition
├── enemy.cpp            # Base Enemy class implementation
├── enemies/             # Enemy types organized by category
│   ├── basic_enemies.h      # GruntEnemy, ScoutEnemy declarations
│   ├── basic_enemies.cpp    # GruntEnemy, ScoutEnemy implementations
│   ├── combat_enemies.h     # BruteEnemy, AssassinEnemy declarations
│   ├── combat_enemies.cpp   # BruteEnemy, AssassinEnemy implementations
│   ├── special_enemies.h    # SniperEnemy, SwarmEnemy declarations
│   ├── special_enemies.cpp  # SniperEnemy, SwarmEnemy implementations
│   ├── enemy_factory.h      # Factory functions and includes
│   └── enemy_factory.cpp    # Factory implementations
├── main.cpp             # Only includes enemy_factory.h
└── minimap.cpp          # Works with Enemy base class pointers
```

## Benefits of This Organization

### ✅ **Modular Development**
- Each enemy category is in its own file pair
- Changes to one enemy type don't require recompiling others
- Easy to work on specific enemy behaviors in isolation

### ✅ **Clean Dependencies**
- `main.cpp` only needs to include `enemy_factory.h`
- Factory handles all enemy type includes internally
- Base `enemy.h` remains lightweight

### ✅ **Scalable Structure**
- Easy to add new categories (e.g., `boss_enemies.h/cpp`)
- Can split large categories into smaller files
- Clear naming convention for organization

### ✅ **Team-Friendly**
- Multiple developers can work on different enemy types simultaneously
- Git merge conflicts are minimized
- Code reviews are more focused

## How to Add New Enemy Types

### Option 1: Add to Existing Category

If your new enemy fits an existing category, add it to the appropriate files:

**Example: Adding `TankEnemy` to combat enemies**

1. **Add declaration to `src/enemies/combat_enemies.h`:**
```cpp
class TankEnemy : public Enemy {
public:
    TankEnemy();
    void updateAI(const sf::Vector2f& playerPos, float deltaTime) override;
    void takeDamage(int damage) override;
private:
    int armor;
    bool shieldActive;
};
```

2. **Add implementation to `src/enemies/combat_enemies.cpp`:**
```cpp
TankEnemy::TankEnemy() : Enemy(), armor(5), shieldActive(false) {
    health = maxHealth = 200;
    speed = 60.0f;
    damage = 15;
    // ... rest of implementation
}
```

3. **Add to factory in `src/enemies/enemy_factory.cpp`:**
```cpp
std::unique_ptr<Enemy> createCombatEnemy() {
    int typeRoll = rand() % 3; // Now 3 types
    
    if (typeRoll == 0) {
        return std::make_unique<BruteEnemy>();
    } else if (typeRoll == 1) {
        return std::make_unique<AssassinEnemy>();
    } else {
        return std::make_unique<TankEnemy>();
    }
}
```

### Option 2: Create New Category

For significantly different enemy types, create a new category:

**Example: Adding boss enemies**

1. **Create `src/enemies/boss_enemies.h`:**
```cpp
#pragma once
#include "../enemy.h"

class DragonBoss : public Enemy {
public:
    DragonBoss();
    void updateAI(const sf::Vector2f& playerPos, float deltaTime) override;
    void attack(Player& player) override;
    void draw(sf::RenderWindow& window) const override;
private:
    enum class Phase { NORMAL, ENRAGED, FLYING };
    Phase currentPhase;
    float phaseTimer;
};
```

2. **Create `src/enemies/boss_enemies.cpp`:**
```cpp
#include "boss_enemies.h"
#include "../config.h"

DragonBoss::DragonBoss() : Enemy(), currentPhase(Phase::NORMAL), phaseTimer(0.0f) {
    health = maxHealth = 500;
    speed = 120.0f;
    damage = 50;
    // ... implementation
}
```

3. **Update `src/enemies/enemy_factory.h`:**
```cpp
#include "boss_enemies.h"

// Add new factory function
std::unique_ptr<Enemy> createBossEnemy();
```

4. **Update `src/enemies/enemy_factory.cpp`:**
```cpp
std::unique_ptr<Enemy> createBossEnemy() {
    return std::make_unique<DragonBoss>();
}
```

5. **Update `CMakeLists.txt`:**
```cmake
set(SOURCES
    # ... existing sources
    src/enemies/boss_enemies.cpp
)

set(HEADERS
    # ... existing headers
    src/enemies/boss_enemies.h
)
```

## Factory Pattern Usage

The factory system provides multiple ways to create enemies:

### Random Enemy Creation
```cpp
// In main.cpp or wherever you spawn enemies
auto enemy = createRandomEnemy(); // Any enemy type
```

### Category-Specific Creation
```cpp
auto basicEnemy = createBasicEnemy();     // Grunt or Scout
auto combatEnemy = createCombatEnemy();   // Brute or Assassin
auto specialEnemy = createSpecialEnemy(); // Sniper or Swarm
```

### Level-Based Spawning Example
```cpp
// Early game - mostly basic enemies
for (int i = 0; i < 10; i++) {
    if (rand() % 100 < 70) {
        enemies.push_back(createBasicEnemy());
    } else {
        enemies.push_back(createCombatEnemy());
    }
}

// Late game - more variety
for (int i = 0; i < 15; i++) {
    enemies.push_back(createRandomEnemy());
}
```

## Compilation Benefits

### Faster Incremental Builds
- Changing `GruntEnemy` only recompiles `basic_enemies.cpp`
- Adding new enemies doesn't require full project rebuild
- Base `Enemy` class changes trigger minimal recompilation

### Parallel Compilation
```bash
# CMake can build enemy files in parallel
cmake --build . -j$(nproc)
```

## Code Organization Patterns

### Naming Conventions
- **Headers:** `category_enemies.h` (e.g., `basic_enemies.h`)
- **Sources:** `category_enemies.cpp` (e.g., `basic_enemies.cpp`)
- **Classes:** `TypeEnemy` (e.g., `GruntEnemy`, `TankEnemy`)

### Include Guards
All headers use `#pragma once` for simplicity and compiler optimization.

### Relative Includes
Enemy files use relative paths:
```cpp
#include "../enemy.h"    // From enemies/ to src/
#include "../config.h"   // Access to game configuration
```

## Advanced Organization Ideas

### Behavior-Based Organization
```
enemies/
├── melee_enemies.h/cpp     # Close combat enemies
├── ranged_enemies.h/cpp    # Long-range enemies
├── support_enemies.h/cpp   # Healers, buffers
└── environmental_enemies.h/cpp # Teleporters, trappers
```

### Tier-Based Organization
```
enemies/
├── tier1_enemies.h/cpp     # Early game
├── tier2_enemies.h/cpp     # Mid game
├── tier3_enemies.h/cpp     # Late game
└── boss_enemies.h/cpp      # Special encounters
```

### Ability-Based Organization
```
enemies/
├── mobile_enemies.h/cpp    # Fast, agile enemies
├── tank_enemies.h/cpp      # High health, slow enemies
├── caster_enemies.h/cpp    # Magic/special ability enemies
└── hybrid_enemies.h/cpp    # Multi-role enemies
```

## Integration with Other Systems

### With Spawning Systems
```cpp
class WaveSpawner {
    std::vector<std::function<std::unique_ptr<Enemy>()>> enemyFactories = {
        createBasicEnemy,
        createCombatEnemy,
        createSpecialEnemy
    };
    
    void spawnWave(int difficulty) {
        auto factory = enemyFactories[difficulty % enemyFactories.size()];
        enemies.push_back(factory());
    }
};
```

### With Data-Driven Configuration
```cpp
// In a future enhancement, you could load enemy configs from JSON
struct EnemySpawnConfig {
    std::string category;
    float spawnWeight;
    int minLevel;
};

std::vector<EnemySpawnConfig> spawnTable = {
    {"basic", 0.4f, 1},
    {"combat", 0.3f, 3},
    {"special", 0.2f, 5},
    {"boss", 0.1f, 10}
};
```

## Migration Benefits

The refactoring maintains 100% compatibility with your existing code while providing:

- **Better organization** for future development
- **Easier debugging** with isolated enemy behaviors
- **Cleaner git history** with focused commits
- **Reduced compilation times** for iterative development
- **Team scalability** for multiple developers

Your enemy system is now perfectly structured to handle dozens or hundreds of enemy types without becoming unwieldy! 