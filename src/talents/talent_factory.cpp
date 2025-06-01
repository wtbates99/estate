#include "talent_factory.h"
#include <random>

std::unique_ptr<Talent> createRandomTalent() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 14);
    
    switch (dis(gen)) {
        // Combat talents (5)
        case 0: return createDamageBoostTalent();
        case 1: return createAttackSpeedTalent();
        case 2: return createCriticalStrikeTalent();
        case 3: return createVampirismTalent();
        case 4: return createBerserkTalent();
        
        // Defensive talents (5)
        case 5: return createHealthBoostTalent();
        case 6: return createArmorTalent();
        case 7: return createRegenerationTalent();
        case 8: return createShieldTalent();
        case 9: return createDodgeTalent();
        
        // Utility talents (5)
        case 10: return createSpeedBoostTalent();
        case 11: return createExperienceBoostTalent();
        case 12: return createGoldFindTalent();
        case 13: return createLuckyDropsTalent();
        case 14: return createMinimapMasteryTalent();
        
        default: return createDamageBoostTalent();
    }
}

std::unique_ptr<Talent> createCombatTalent() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 4);
    
    switch (dis(gen)) {
        case 0: return createDamageBoostTalent();
        case 1: return createAttackSpeedTalent();
        case 2: return createCriticalStrikeTalent();
        case 3: return createVampirismTalent();
        case 4: return createBerserkTalent();
        default: return createDamageBoostTalent();
    }
}

std::unique_ptr<Talent> createDefensiveTalent() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 4);
    
    switch (dis(gen)) {
        case 0: return createHealthBoostTalent();
        case 1: return createArmorTalent();
        case 2: return createRegenerationTalent();
        case 3: return createShieldTalent();
        case 4: return createDodgeTalent();
        default: return createHealthBoostTalent();
    }
}

std::unique_ptr<Talent> createUtilityTalent() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 4);
    
    switch (dis(gen)) {
        case 0: return createSpeedBoostTalent();
        case 1: return createExperienceBoostTalent();
        case 2: return createGoldFindTalent();
        case 3: return createLuckyDropsTalent();
        case 4: return createMinimapMasteryTalent();
        default: return createSpeedBoostTalent();
    }
}

// Combat talent factories
std::unique_ptr<Talent> createDamageBoostTalent() {
    return std::make_unique<DamageBoostTalent>();
}

std::unique_ptr<Talent> createAttackSpeedTalent() {
    return std::make_unique<AttackSpeedTalent>();
}

std::unique_ptr<Talent> createCriticalStrikeTalent() {
    return std::make_unique<CriticalStrikeTalent>();
}

std::unique_ptr<Talent> createVampirismTalent() {
    return std::make_unique<VampirismTalent>();
}

std::unique_ptr<Talent> createBerserkTalent() {
    return std::make_unique<BerserkTalent>();
}

std::unique_ptr<Talent> createWeaponMasteryTalent() {
    return std::make_unique<WeaponMasteryTalent>();
}

std::unique_ptr<Talent> createDoubleStrikeTalent() {
    return std::make_unique<DoubleStrikeTalent>();
}

std::unique_ptr<Talent> createExecutionerTalent() {
    return std::make_unique<ExecutionerTalent>();
}

// Defensive talent factories
std::unique_ptr<Talent> createHealthBoostTalent() {
    return std::make_unique<HealthBoostTalent>();
}

std::unique_ptr<Talent> createArmorTalent() {
    return std::make_unique<ArmorTalent>();
}

std::unique_ptr<Talent> createRegenerationTalent() {
    return std::make_unique<RegenerationTalent>();
}

std::unique_ptr<Talent> createShieldTalent() {
    return std::make_unique<ShieldTalent>();
}

std::unique_ptr<Talent> createDodgeTalent() {
    return std::make_unique<DodgeTalent>();
}

// Utility talent factories
std::unique_ptr<Talent> createSpeedBoostTalent() {
    return std::make_unique<SpeedBoostTalent>();
}

std::unique_ptr<Talent> createExperienceBoostTalent() {
    return std::make_unique<ExperienceBoostTalent>();
}

std::unique_ptr<Talent> createGoldFindTalent() {
    return std::make_unique<GoldFindTalent>();
}

std::unique_ptr<Talent> createLuckyDropsTalent() {
    return std::make_unique<LuckyDropsTalent>();
}

std::unique_ptr<Talent> createMinimapMasteryTalent() {
    return std::make_unique<MinimapMasteryTalent>();
} 