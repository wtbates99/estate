#pragma once
#include "../talent.h"

// Combat talent implementations
class DamageBoostTalent : public Talent {
public:
    DamageBoostTalent();
    void apply(Player& player) override;
    void onLevelUp(Player& player) override;
    std::unique_ptr<Talent> createUpgrade() const override;

private:
    float damageMultiplier_;
};

class AttackSpeedTalent : public Talent {
public:
    AttackSpeedTalent();
    void apply(Player& player) override;
    void onLevelUp(Player& player) override;
    std::unique_ptr<Talent> createUpgrade() const override;

private:
    float speedBonus_;
};

class CriticalStrikeTalent : public Talent {
public:
    CriticalStrikeTalent();
    void apply(Player& player) override;
    void onLevelUp(Player& player) override;
    std::unique_ptr<Talent> createUpgrade() const override;

private:
    float critChance_;
    float critMultiplier_;
};

class VampirismTalent : public Talent {
public:
    VampirismTalent();
    void apply(Player& player) override;
    void onLevelUp(Player& player) override;
    std::unique_ptr<Talent> createUpgrade() const override;

private:
    float lifeStealPercent_;
};

class BerserkTalent : public Talent {
public:
    BerserkTalent();
    void apply(Player& player) override;
    void onLevelUp(Player& player) override;
    std::unique_ptr<Talent> createUpgrade() const override;

private:
    float lowHealthThreshold_;
    float damageBonus_;
};

// Additional combat talents
class WeaponMasteryTalent : public Talent {
public:
    WeaponMasteryTalent();
    void apply(Player& player) override;
    void onLevelUp(Player& player) override;
    std::unique_ptr<Talent> createUpgrade() const override;

private:
    float weaponDamageBonus_;
};

class DoubleStrikeTalent : public Talent {
public:
    DoubleStrikeTalent();
    void apply(Player& player) override;
    void onLevelUp(Player& player) override;
    std::unique_ptr<Talent> createUpgrade() const override;

private:
    float doubleStrikeChance_;
};

class ExecutionerTalent : public Talent {
public:
    ExecutionerTalent();
    void apply(Player& player) override;
    void onLevelUp(Player& player) override;
    std::unique_ptr<Talent> createUpgrade() const override;

private:
    float executeThreshold_;
    float bonusDamage_;
}; 