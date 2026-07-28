#include "defensive_talents.h"
#include "../player.h"
#include <sstream>

// HealthBoostTalent implementation
HealthBoostTalent::HealthBoostTalent()
    : Talent("Health Boost", "Increases maximum health", TalentCategory::DEFENSIVE, TalentRarity::COMMON),
      healthBonus_(25) {
}

void HealthBoostTalent::apply(Player& player) {
    int newMaxHealth = player.getMaxHealth() + healthBonus_;
    player.setMaxHealth(newMaxHealth);
    player.healPlayer(healthBonus_); // Also heal the player
}

void HealthBoostTalent::onLevelUp(Player& player) {
    healthBonus_ += 15;
    
    std::ostringstream desc;
    desc << "Increases maximum health by " << healthBonus_;
    description_ = desc.str();
    
    // Apply the additional health
    player.setMaxHealth(player.getMaxHealth() + 15);
    player.healPlayer(15);
}

std::unique_ptr<Talent> HealthBoostTalent::createUpgrade() const {
    auto upgrade = std::make_unique<HealthBoostTalent>();
    upgrade->level_ = level_ + 1;
    upgrade->healthBonus_ = healthBonus_ + 15;
    upgrade->rarity_ = TalentRarity::UNCOMMON;
    upgrade->name_ = "Vitality";
    return std::move(upgrade);
}

// ArmorTalent implementation
ArmorTalent::ArmorTalent()
    : Talent("Armor", "Reduces incoming damage", TalentCategory::DEFENSIVE, TalentRarity::UNCOMMON),
      armorPoints_(2) {
}

void ArmorTalent::apply(Player& player) {
    player.addArmor(static_cast<float>(armorPoints_));
}

void ArmorTalent::onLevelUp(Player& player) {
    armorPoints_ += 1;
    
    std::ostringstream desc;
    desc << "Reduces damage by " << armorPoints_ << " points";
    description_ = desc.str();
}

std::unique_ptr<Talent> ArmorTalent::createUpgrade() const {
    auto upgrade = std::make_unique<ArmorTalent>();
    upgrade->level_ = level_ + 1;
    upgrade->armorPoints_ = armorPoints_ + 1;
    upgrade->rarity_ = TalentRarity::RARE;
    upgrade->name_ = "Plate Armor";
    return std::move(upgrade);
}

// RegenerationTalent implementation
RegenerationTalent::RegenerationTalent()
    : Talent("Regeneration", "Slowly heals over time", TalentCategory::DEFENSIVE, TalentRarity::RARE),
      regenRate_(1.0f) {
}

void RegenerationTalent::apply(Player& player) {
    float regenPercent = regenRate_ / 100.0f; // Convert to percentage of max health
    player.addRegenRate(regenPercent);
}

void RegenerationTalent::onLevelUp(Player& player) {
    regenRate_ += 0.5f;
    
    std::ostringstream desc;
    desc << "Regenerates " << regenRate_ << " health per second";
    description_ = desc.str();
}

std::unique_ptr<Talent> RegenerationTalent::createUpgrade() const {
    auto upgrade = std::make_unique<RegenerationTalent>();
    upgrade->level_ = level_ + 1;
    upgrade->regenRate_ = regenRate_ + 0.5f;
    upgrade->rarity_ = TalentRarity::EPIC;
    upgrade->name_ = "Fast Healing";
    return std::move(upgrade);
}

// ShieldTalent implementation
ShieldTalent::ShieldTalent()
    : Talent("Shield", "Absorbs damage before health", TalentCategory::DEFENSIVE, TalentRarity::EPIC),
      shieldCapacity_(50), rechargeRate_(5.0f) {
}

void ShieldTalent::apply(Player& player) {
    // Implementation would add shield mechanics
}

void ShieldTalent::onLevelUp(Player& player) {
    shieldCapacity_ += 25;
    rechargeRate_ += 2.0f;
    
    std::ostringstream desc;
    desc << shieldCapacity_ << " shield points, recharges " << rechargeRate_ << "/sec";
    description_ = desc.str();
}

std::unique_ptr<Talent> ShieldTalent::createUpgrade() const {
    auto upgrade = std::make_unique<ShieldTalent>();
    upgrade->level_ = level_ + 1;
    upgrade->shieldCapacity_ = shieldCapacity_ + 25;
    upgrade->rechargeRate_ = rechargeRate_ + 2.0f;
    upgrade->rarity_ = TalentRarity::LEGENDARY;
    upgrade->name_ = "Energy Shield";
    return std::move(upgrade);
}

// DodgeTalent implementation
DodgeTalent::DodgeTalent()
    : Talent("Dodge", "Chance to avoid damage", TalentCategory::DEFENSIVE, TalentRarity::RARE),
      dodgeChance_(0.1f) {
}

void DodgeTalent::apply(Player& player) {
    player.addDodgeChance(dodgeChance_);
}

void DodgeTalent::onLevelUp(Player& player) {
    dodgeChance_ += 0.05f;
    
    std::ostringstream desc;
    desc << (dodgeChance_ * 100.0f) << "% chance to dodge attacks";
    description_ = desc.str();
}

std::unique_ptr<Talent> DodgeTalent::createUpgrade() const {
    auto upgrade = std::make_unique<DodgeTalent>();
    upgrade->level_ = level_ + 1;
    upgrade->dodgeChance_ = dodgeChance_ + 0.05f;
    upgrade->rarity_ = TalentRarity::EPIC;
    upgrade->name_ = "Evasion Master";
    return std::move(upgrade);
} 