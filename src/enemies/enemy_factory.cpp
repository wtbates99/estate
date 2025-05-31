#include "enemy_factory.h"

// Main factory function for creating random enemies
std::unique_ptr<Enemy> createRandomEnemy() {
    int typeRoll = rand() % 100;
    
    if (typeRoll < 30) {
        return std::make_unique<GruntEnemy>();
    } else if (typeRoll < 45) {
        return std::make_unique<ScoutEnemy>();
    } else if (typeRoll < 60) {
        return std::make_unique<BruteEnemy>();
    } else if (typeRoll < 75) {
        return std::make_unique<AssassinEnemy>();
    } else if (typeRoll < 85) {
        return std::make_unique<SniperEnemy>();
    } else {
        return std::make_unique<SwarmEnemy>();
    }
}

// Factory function for basic enemies only
std::unique_ptr<Enemy> createBasicEnemy() {
    int typeRoll = rand() % 2;
    
    if (typeRoll == 0) {
        return std::make_unique<GruntEnemy>();
    } else {
        return std::make_unique<ScoutEnemy>();
    }
}

// Factory function for combat enemies only
std::unique_ptr<Enemy> createCombatEnemy() {
    int typeRoll = rand() % 2;
    
    if (typeRoll == 0) {
        return std::make_unique<BruteEnemy>();
    } else {
        return std::make_unique<AssassinEnemy>();
    }
}

// Factory function for special enemies only
std::unique_ptr<Enemy> createSpecialEnemy() {
    int typeRoll = rand() % 2;
    
    if (typeRoll == 0) {
        return std::make_unique<SniperEnemy>();
    } else {
        return std::make_unique<SwarmEnemy>();
    }
} 