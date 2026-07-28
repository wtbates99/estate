#pragma once
#include <memory>
#include "../talent.h"
#include "combat_talents.h"
#include "defensive_talents.h"
#include "utility_talents.h"

// Factory function for creating random talents
std::unique_ptr<Talent> createRandomTalent();

// Factory functions for specific categories
std::unique_ptr<Talent> createCombatTalent();
std::unique_ptr<Talent> createDefensiveTalent();
std::unique_ptr<Talent> createUtilityTalent();

// Factory functions for specific talents
// Combat talents
std::unique_ptr<Talent> createDamageBoostTalent();
std::unique_ptr<Talent> createAttackSpeedTalent();
std::unique_ptr<Talent> createCriticalStrikeTalent();
std::unique_ptr<Talent> createVampirismTalent();
std::unique_ptr<Talent> createBerserkTalent();
std::unique_ptr<Talent> createWeaponMasteryTalent();
std::unique_ptr<Talent> createDoubleStrikeTalent();
std::unique_ptr<Talent> createExecutionerTalent();

// Defensive talents
std::unique_ptr<Talent> createHealthBoostTalent();
std::unique_ptr<Talent> createArmorTalent();
std::unique_ptr<Talent> createRegenerationTalent();
std::unique_ptr<Talent> createShieldTalent();
std::unique_ptr<Talent> createDodgeTalent();

// Utility talents
std::unique_ptr<Talent> createSpeedBoostTalent();
std::unique_ptr<Talent> createExperienceBoostTalent();
std::unique_ptr<Talent> createGoldFindTalent();
std::unique_ptr<Talent> createLuckyDropsTalent();
std::unique_ptr<Talent> createMinimapMasteryTalent(); 