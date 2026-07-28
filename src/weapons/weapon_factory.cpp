#include "weapon_factory.h"
#include <random>

std::unique_ptr<Weapon> createRandomWeapon() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 3);
    
    switch (dis(gen)) {
        case 0: return createSword();
        case 1: return createDagger();
        case 2: return createBow();
        case 3: return createCrossbow();
        default: return createSword();
    }
}

std::unique_ptr<Weapon> createMeleeWeapon() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 1);
    
    switch (dis(gen)) {
        case 0: return createSword();
        case 1: return createDagger();
        default: return createSword();
    }
}

std::unique_ptr<Weapon> createRangedWeapon() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 1);
    
    switch (dis(gen)) {
        case 0: return createBow();
        case 1: return createCrossbow();
        default: return createBow();
    }
}

std::unique_ptr<Weapon> createSword() {
    return std::make_unique<Sword>();
}

std::unique_ptr<Weapon> createDagger() {
    return std::make_unique<Dagger>();
}

std::unique_ptr<Weapon> createBow() {
    return std::make_unique<Bow>();
}

std::unique_ptr<Weapon> createCrossbow() {
    return std::make_unique<Crossbow>();
} 