#pragma once
#include "../talent.h"

// Defensive talent implementations
class HealthBoostTalent : public Talent {
public:
    HealthBoostTalent();
    void apply(Player& player) override;
    void onLevelUp(Player& player) override;
    std::unique_ptr<Talent> createUpgrade() const override;

private:
    int healthBonus_;
};

class ArmorTalent : public Talent {
public:
    ArmorTalent();
    void apply(Player& player) override;
    void onLevelUp(Player& player) override;
    std::unique_ptr<Talent> createUpgrade() const override;

private:
    int armorPoints_;
};

class RegenerationTalent : public Talent {
public:
    RegenerationTalent();
    void apply(Player& player) override;
    void onLevelUp(Player& player) override;
    std::unique_ptr<Talent> createUpgrade() const override;

private:
    float regenRate_;
};

class ShieldTalent : public Talent {
public:
    ShieldTalent();
    void apply(Player& player) override;
    void onLevelUp(Player& player) override;
    std::unique_ptr<Talent> createUpgrade() const override;

private:
    int shieldCapacity_;
    float rechargeRate_;
};

class DodgeTalent : public Talent {
public:
    DodgeTalent();
    void apply(Player& player) override;
    void onLevelUp(Player& player) override;
    std::unique_ptr<Talent> createUpgrade() const override;

private:
    float dodgeChance_;
}; 