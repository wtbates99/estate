#pragma once
#include <memory>
#include "../weapon.h"
#include "melee_weapons.h"
#include "ranged_weapons.h"

// Factory function for creating random weapons
std::unique_ptr<Weapon> createRandomWeapon();

// Factory functions for specific categories
std::unique_ptr<Weapon> createMeleeWeapon();
std::unique_ptr<Weapon> createRangedWeapon();

// Factory functions for specific weapons
std::unique_ptr<Weapon> createSword();
std::unique_ptr<Weapon> createDagger();
std::unique_ptr<Weapon> createBow();
std::unique_ptr<Weapon> createCrossbow(); 