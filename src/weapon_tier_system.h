#pragma once
#include <string>
#include <vector>
#include <memory>
#include "weapon.h"

enum class WeaponTier {
    BASIC = 0,
    ADVANCED = 1,
    LEGENDARY = 2
};

enum class WeaponCategory {
    // Classic Weapons
    SWORD,
    DAGGER,
    BOW,
    CROSSBOW,
    AXE,
    STAFF,
    HAMMER,
    SPEAR,
    WAND,
    CANNON,
    
    // Badass Weapons
    CHAINSAW,
    ROCKET_LAUNCHER,
    FLAMETHROWER,
    MINIGUN,
    LIGHTNING_ROD,
    SOUL_REAPER,
    DEMON_CLAW,
    VOID_BLADE,
    CRYSTAL_LAUNCHER,
    STORM_HAMMER,
    
    // Funny Weapons
    RUBBER_CHICKEN,
    BANANA_LAUNCHER,
    FISH_SLAPPER,
    POOL_NOODLE,
    FEATHER_DUSTER,
    SQUIRT_GUN,
    PILLOW_CANNON,
    RUBBER_DUCK_BOMBS,
    PIZZA_SLICER,
    SOCK_LAUNCHER,
    
    // Mythical/Epic Weapons  
    EXCALIBUR,
    MJOLNIR,
    DRAGONSLAYER,
    SHADOWBANE,
    FROSTMOURNE,
    FLAMBERGE,
    WORLD_ENDER,
    GALAXY_PIERCER,
    TIME_RIPPER,
    REALITY_SHARD,
    
    // Modern/Sci-Fi Weapons
    PLASMA_RIFLE,
    LASER_CANNON,
    GRAVITY_GUN,
    QUANTUM_BLASTER,
    ION_SWORD,
    NANO_SWARM,
    TESLA_COIL,
    PHOTON_LANCE,
    ANTIMATTER_BOMB,
    BLACK_HOLE_GUN
};

struct WeaponStats {
    int baseDamage;
    float baseCooldown;
    float baseRange;
    int cost;
    std::string name;
    std::string description;
    
    WeaponStats(int dmg, float cd, float rng, int c, const std::string& n, const std::string& desc)
        : baseDamage(dmg), baseCooldown(cd), baseRange(rng), cost(c), name(n), description(desc) {}
};

struct WeaponUpgrade {
    int level; // 1, 2, 3
    std::string suffix;
    float damageMultiplier;
    float cooldownMultiplier;
    float rangeMultiplier;
    int upgradeCost;
    std::string description;
    
    WeaponUpgrade(int lvl, const std::string& suf, float dmgMult, float cdMult, float rngMult, int cost, const std::string& desc)
        : level(lvl), suffix(suf), damageMultiplier(dmgMult), cooldownMultiplier(cdMult), 
          rangeMultiplier(rngMult), upgradeCost(cost), description(desc) {}
};

class WeaponTierSystem {
public:
    static WeaponTierSystem& getInstance();
    
    // Get weapon stats for a specific tier and category
    WeaponStats getWeaponStats(WeaponCategory category, WeaponTier tier) const;
    
    // Get upgrade information
    WeaponUpgrade getUpgrade(int level) const;
    
    // Calculate final stats with upgrades
    WeaponStats calculateUpgradedStats(const WeaponStats& base, const WeaponUpgrade& upgrade) const;
    
    // Create weapon with specific tier and upgrade level
    std::unique_ptr<Weapon> createWeapon(WeaponCategory category, WeaponTier tier, int upgradeLevel = 0) const;
    
    // Get all weapons for a tier (for shop display)
    std::vector<WeaponStats> getWeaponsForTier(WeaponTier tier) const;
    
    // Utility functions
    std::string getTierName(WeaponTier tier) const;
    std::string getCategoryName(WeaponCategory category) const;
    std::vector<WeaponCategory> getAllCategories() const;
    
private:
    WeaponTierSystem();
    void initializeWeaponStats();
    void initializeUpgrades();
    
    // Storage for all weapon configurations
    std::vector<std::vector<WeaponStats>> weaponStats_; // [category][tier]
    std::vector<WeaponUpgrade> upgrades_; // [level-1]
};