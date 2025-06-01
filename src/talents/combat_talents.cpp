#include "combat_talents.h"
#include "../player.h"
#include <sstream>

// DamageBoostTalent implementation
DamageBoostTalent::DamageBoostTalent() 
    : Talent("Damage Boost", "Increases weapon damage", TalentCategory::COMBAT, TalentRarity::COMMON),
      damageMultiplier_(1.15f) {
}

void DamageBoostTalent::apply(Player& player) {
    // For now, we'll track this in the talent itself
    // In a full implementation, we'd modify weapon damage through the player
}

void DamageBoostTalent::onLevelUp(Player& player) {
    damageMultiplier_ += 0.05f; // Additional 5% per level
    
    // Update description to show current bonus
    std::ostringstream desc;
    desc << "Increases weapon damage by " << ((damageMultiplier_ - 1.0f) * 100.0f) << "%";
    description_ = desc.str();
}

std::unique_ptr<Talent> DamageBoostTalent::createUpgrade() const {
    auto upgrade = std::make_unique<DamageBoostTalent>();
    upgrade->level_ = level_ + 1;
    upgrade->damageMultiplier_ = damageMultiplier_ + 0.05f;
    upgrade->rarity_ = TalentRarity::UNCOMMON;
    upgrade->name_ = "Enhanced Damage";
    return std::move(upgrade);
}

// AttackSpeedTalent implementation
AttackSpeedTalent::AttackSpeedTalent()
    : Talent("Attack Speed", "Increases attack speed", TalentCategory::COMBAT, TalentRarity::COMMON),
      speedBonus_(0.15f) {
}

void AttackSpeedTalent::apply(Player& player) {
    // Implementation would modify weapon attack cooldown
}

void AttackSpeedTalent::onLevelUp(Player& player) {
    speedBonus_ += 0.1f;
    
    std::ostringstream desc;
    desc << "Increases attack speed by " << (speedBonus_ * 100.0f) << "%";
    description_ = desc.str();
}

std::unique_ptr<Talent> AttackSpeedTalent::createUpgrade() const {
    auto upgrade = std::make_unique<AttackSpeedTalent>();
    upgrade->level_ = level_ + 1;
    upgrade->speedBonus_ = speedBonus_ + 0.1f;
    upgrade->rarity_ = TalentRarity::UNCOMMON;
    upgrade->name_ = "Lightning Strikes";
    return std::move(upgrade);
}

// CriticalStrikeTalent implementation
CriticalStrikeTalent::CriticalStrikeTalent()
    : Talent("Critical Strike", "Chance for critical hits", TalentCategory::COMBAT, TalentRarity::UNCOMMON),
      critChance_(0.1f), critMultiplier_(2.0f) {
}

void CriticalStrikeTalent::apply(Player& player) {
    // Implementation would add critical strike mechanics
}

void CriticalStrikeTalent::onLevelUp(Player& player) {
    critChance_ += 0.05f;
    critMultiplier_ += 0.25f;
    
    std::ostringstream desc;
    desc << critChance_ * 100.0f << "% chance for " << critMultiplier_ << "x damage critical hits";
    description_ = desc.str();
}

std::unique_ptr<Talent> CriticalStrikeTalent::createUpgrade() const {
    auto upgrade = std::make_unique<CriticalStrikeTalent>();
    upgrade->level_ = level_ + 1;
    upgrade->critChance_ = critChance_ + 0.05f;
    upgrade->critMultiplier_ = critMultiplier_ + 0.25f;
    upgrade->rarity_ = TalentRarity::RARE;
    upgrade->name_ = "Lethal Precision";
    return std::move(upgrade);
}

// VampirismTalent implementation
VampirismTalent::VampirismTalent()
    : Talent("Vampirism", "Heals on enemy kill", TalentCategory::COMBAT, TalentRarity::RARE),
      lifeStealPercent_(0.1f) {
}

void VampirismTalent::apply(Player& player) {
    // Implementation would add life steal mechanics
}

void VampirismTalent::onLevelUp(Player& player) {
    lifeStealPercent_ += 0.05f;
    
    std::ostringstream desc;
    desc << "Heal for " << (lifeStealPercent_ * 100.0f) << "% of damage dealt";
    description_ = desc.str();
}

std::unique_ptr<Talent> VampirismTalent::createUpgrade() const {
    auto upgrade = std::make_unique<VampirismTalent>();
    upgrade->level_ = level_ + 1;
    upgrade->lifeStealPercent_ = lifeStealPercent_ + 0.05f;
    upgrade->rarity_ = TalentRarity::EPIC;
    upgrade->name_ = "Blood Drinker";
    return std::move(upgrade);
}

// BerserkTalent implementation
BerserkTalent::BerserkTalent()
    : Talent("Berserk", "More damage at low health", TalentCategory::COMBAT, TalentRarity::EPIC),
      lowHealthThreshold_(0.3f), damageBonus_(0.5f) {
}

void BerserkTalent::apply(Player& player) {
    // Implementation would check health and modify damage
}

void BerserkTalent::onLevelUp(Player& player) {
    damageBonus_ += 0.25f;
    lowHealthThreshold_ += 0.1f; // Triggers at higher health
    
    std::ostringstream desc;
    desc << "+" << (damageBonus_ * 100.0f) << "% damage when below " << (lowHealthThreshold_ * 100.0f) << "% health";
    description_ = desc.str();
}

std::unique_ptr<Talent> BerserkTalent::createUpgrade() const {
    auto upgrade = std::make_unique<BerserkTalent>();
    upgrade->level_ = level_ + 1;
    upgrade->damageBonus_ = damageBonus_ + 0.25f;
    upgrade->lowHealthThreshold_ = lowHealthThreshold_ + 0.1f;
    upgrade->rarity_ = TalentRarity::LEGENDARY;
    upgrade->name_ = "Unstoppable Rage";
    return std::move(upgrade);
}

// WeaponMasteryTalent implementation
WeaponMasteryTalent::WeaponMasteryTalent()
    : Talent("Weapon Mastery", "Bonus damage with current weapon type", TalentCategory::COMBAT, TalentRarity::UNCOMMON),
      weaponDamageBonus_(0.25f) {
}

void WeaponMasteryTalent::apply(Player& player) {
    // Implementation would add weapon-specific bonuses
}

void WeaponMasteryTalent::onLevelUp(Player& player) {
    weaponDamageBonus_ += 0.15f;
    
    std::ostringstream desc;
    desc << "+" << (weaponDamageBonus_ * 100.0f) << "% damage with current weapon type";
    description_ = desc.str();
}

std::unique_ptr<Talent> WeaponMasteryTalent::createUpgrade() const {
    auto upgrade = std::make_unique<WeaponMasteryTalent>();
    upgrade->level_ = level_ + 1;
    upgrade->weaponDamageBonus_ = weaponDamageBonus_ + 0.15f;
    upgrade->rarity_ = TalentRarity::RARE;
    upgrade->name_ = "Master Combatant";
    return std::move(upgrade);
}

// DoubleStrikeTalent implementation
DoubleStrikeTalent::DoubleStrikeTalent()
    : Talent("Double Strike", "Chance to strike twice", TalentCategory::COMBAT, TalentRarity::RARE),
      doubleStrikeChance_(0.15f) {
}

void DoubleStrikeTalent::apply(Player& player) {
    // Implementation would add double strike mechanics
}

void DoubleStrikeTalent::onLevelUp(Player& player) {
    doubleStrikeChance_ += 0.1f;
    
    std::ostringstream desc;
    desc << (doubleStrikeChance_ * 100.0f) << "% chance to strike twice";
    description_ = desc.str();
}

std::unique_ptr<Talent> DoubleStrikeTalent::createUpgrade() const {
    auto upgrade = std::make_unique<DoubleStrikeTalent>();
    upgrade->level_ = level_ + 1;
    upgrade->doubleStrikeChance_ = doubleStrikeChance_ + 0.1f;
    upgrade->rarity_ = TalentRarity::EPIC;
    upgrade->name_ = "Flurry of Blows";
    return std::move(upgrade);
}

// ExecutionerTalent implementation
ExecutionerTalent::ExecutionerTalent()
    : Talent("Executioner", "Massive damage to low health enemies", TalentCategory::COMBAT, TalentRarity::EPIC),
      executeThreshold_(0.2f), bonusDamage_(2.0f) {
}

void ExecutionerTalent::apply(Player& player) {
    // Implementation would add execution mechanics
}

void ExecutionerTalent::onLevelUp(Player& player) {
    executeThreshold_ += 0.05f;
    bonusDamage_ += 0.5f;
    
    std::ostringstream desc;
    desc << "+" << (bonusDamage_ * 100.0f) << "% damage to enemies below " << (executeThreshold_ * 100.0f) << "% health";
    description_ = desc.str();
}

std::unique_ptr<Talent> ExecutionerTalent::createUpgrade() const {
    auto upgrade = std::make_unique<ExecutionerTalent>();
    upgrade->level_ = level_ + 1;
    upgrade->executeThreshold_ = executeThreshold_ + 0.05f;
    upgrade->bonusDamage_ = bonusDamage_ + 0.5f;
    upgrade->rarity_ = TalentRarity::LEGENDARY;
    upgrade->name_ = "Angel of Death";
    return std::move(upgrade);
} 