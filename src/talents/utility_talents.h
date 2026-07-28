#pragma once
#include "../talent.h"

// Utility talent implementations
class SpeedBoostTalent : public Talent {
public:
    SpeedBoostTalent();
    void apply(Player& player) override;
    void onLevelUp(Player& player) override;
    std::unique_ptr<Talent> createUpgrade() const override;

private:
    float speedMultiplier_;
};

class ExperienceBoostTalent : public Talent {
public:
    ExperienceBoostTalent();
    void apply(Player& player) override;
    void onLevelUp(Player& player) override;
    std::unique_ptr<Talent> createUpgrade() const override;

private:
    float expMultiplier_;
};

class GoldFindTalent : public Talent {
public:
    GoldFindTalent();
    void apply(Player& player) override;
    void onLevelUp(Player& player) override;
    std::unique_ptr<Talent> createUpgrade() const override;

private:
    float goldMultiplier_;
};

class LuckyDropsTalent : public Talent {
public:
    LuckyDropsTalent();
    void apply(Player& player) override;
    void onLevelUp(Player& player) override;
    std::unique_ptr<Talent> createUpgrade() const override;

private:
    float dropChanceBonus_;
};

class MinimapMasteryTalent : public Talent {
public:
    MinimapMasteryTalent();
    void apply(Player& player) override;
    void onLevelUp(Player& player) override;
    std::unique_ptr<Talent> createUpgrade() const override;

private:
    float visionRange_;
}; 