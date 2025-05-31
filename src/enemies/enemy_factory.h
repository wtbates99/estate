#pragma once
#include <memory>
#include "../enemy.h"
#include "basic_enemies.h"
#include "combat_enemies.h"
#include "special_enemies.h"

// Factory function for creating random enemies
std::unique_ptr<Enemy> createRandomEnemy();

// Factory functions for specific categories
std::unique_ptr<Enemy> createBasicEnemy();
std::unique_ptr<Enemy> createCombatEnemy();
std::unique_ptr<Enemy> createSpecialEnemy(); 