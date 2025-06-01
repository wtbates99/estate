#include "utility_talents.h"
#include "../player.h"
#include <sstream>

// SpeedBoostTalent implementation
SpeedBoostTalent::SpeedBoostTalent()
    : Talent("Speed Boost", "Increases movement speed", TalentCategory::UTILITY, TalentRarity::COMMON),
      speedMultiplier_(1.2f) {
}

void SpeedBoostTalent::apply(Player& player) {
    float newSpeed = player.getSpeed() * speedMultiplier_;
    player.setSpeed(newSpeed);
}

void SpeedBoostTalent::onLevelUp(Player& player) {
    // Remove old speed bonus and apply new one
    float baseSpeed = player.getSpeed() / speedMultiplier_;
    speedMultiplier_ += 0.1f;
    player.setSpeed(baseSpeed * speedMultiplier_);
    
    std::ostringstream desc;
    desc << "Increases movement speed by " << ((speedMultiplier_ - 1.0f) * 100.0f) << "%";
    description_ = desc.str();
}

std::unique_ptr<Talent> SpeedBoostTalent::createUpgrade() const {
    auto upgrade = std::make_unique<SpeedBoostTalent>();
    upgrade->level_ = level_ + 1;
    upgrade->speedMultiplier_ = speedMultiplier_ + 0.1f;
    upgrade->rarity_ = TalentRarity::UNCOMMON;
    upgrade->name_ = "Swift Movement";
    return std::move(upgrade);
}

// ExperienceBoostTalent implementation
ExperienceBoostTalent::ExperienceBoostTalent()
    : Talent("Experience Boost", "Gain more experience", TalentCategory::UTILITY, TalentRarity::UNCOMMON),
      expMultiplier_(1.25f) {
}

void ExperienceBoostTalent::apply(Player& player) {
    // Implementation would modify experience gain mechanics
}

void ExperienceBoostTalent::onLevelUp(Player& player) {
    expMultiplier_ += 0.15f;
    
    std::ostringstream desc;
    desc << "Gain " << ((expMultiplier_ - 1.0f) * 100.0f) << "% more experience";
    description_ = desc.str();
}

std::unique_ptr<Talent> ExperienceBoostTalent::createUpgrade() const {
    auto upgrade = std::make_unique<ExperienceBoostTalent>();
    upgrade->level_ = level_ + 1;
    upgrade->expMultiplier_ = expMultiplier_ + 0.15f;
    upgrade->rarity_ = TalentRarity::RARE;
    upgrade->name_ = "Scholar";
    return std::move(upgrade);
}

// GoldFindTalent implementation
GoldFindTalent::GoldFindTalent()
    : Talent("Gold Find", "Find more gold", TalentCategory::UTILITY, TalentRarity::COMMON),
      goldMultiplier_(1.3f) {
}

void GoldFindTalent::apply(Player& player) {
    // Implementation would modify gold gain mechanics
}

void GoldFindTalent::onLevelUp(Player& player) {
    goldMultiplier_ += 0.2f;
    
    std::ostringstream desc;
    desc << "Find " << ((goldMultiplier_ - 1.0f) * 100.0f) << "% more gold";
    description_ = desc.str();
}

std::unique_ptr<Talent> GoldFindTalent::createUpgrade() const {
    auto upgrade = std::make_unique<GoldFindTalent>();
    upgrade->level_ = level_ + 1;
    upgrade->goldMultiplier_ = goldMultiplier_ + 0.2f;
    upgrade->rarity_ = TalentRarity::UNCOMMON;
    upgrade->name_ = "Treasure Hunter";
    return std::move(upgrade);
}

// LuckyDropsTalent implementation
LuckyDropsTalent::LuckyDropsTalent()
    : Talent("Lucky Drops", "Better item drops", TalentCategory::UTILITY, TalentRarity::RARE),
      dropChanceBonus_(0.15f) {
}

void LuckyDropsTalent::apply(Player& player) {
    // Implementation would modify drop chance mechanics
}

void LuckyDropsTalent::onLevelUp(Player& player) {
    dropChanceBonus_ += 0.1f;
    
    std::ostringstream desc;
    desc << "+" << (dropChanceBonus_ * 100.0f) << "% better item drop chances";
    description_ = desc.str();
}

std::unique_ptr<Talent> LuckyDropsTalent::createUpgrade() const {
    auto upgrade = std::make_unique<LuckyDropsTalent>();
    upgrade->level_ = level_ + 1;
    upgrade->dropChanceBonus_ = dropChanceBonus_ + 0.1f;
    upgrade->rarity_ = TalentRarity::EPIC;
    upgrade->name_ = "Fortune's Favor";
    return std::move(upgrade);
}

// MinimapMasteryTalent implementation
MinimapMasteryTalent::MinimapMasteryTalent()
    : Talent("Minimap Mastery", "Improved minimap vision", TalentCategory::UTILITY, TalentRarity::UNCOMMON),
      visionRange_(1.5f) {
}

void MinimapMasteryTalent::apply(Player& player) {
    // Implementation would modify minimap range
}

void MinimapMasteryTalent::onLevelUp(Player& player) {
    visionRange_ += 0.5f;
    
    std::ostringstream desc;
    desc << "Minimap vision range x" << visionRange_;
    description_ = desc.str();
}

std::unique_ptr<Talent> MinimapMasteryTalent::createUpgrade() const {
    auto upgrade = std::make_unique<MinimapMasteryTalent>();
    upgrade->level_ = level_ + 1;
    upgrade->visionRange_ = visionRange_ + 0.5f;
    upgrade->rarity_ = TalentRarity::RARE;
    upgrade->name_ = "All-Seeing Eye";
    return std::move(upgrade);
} 