#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <vector>
#include "config.h"
#include "weapon.h"
#include "talent.h"

// Forward declarations
class Enemy;

class Player {
public:
    Player();
    void move(float deltaTime);
    void takeDamage(int damage);
    void gainExperience(int exp);
    void addGold(int amount);
    bool isAlive() const;
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
    sf::Vector2f getPosition() const { return shape.getPosition(); }
    sf::Vector2f getWorldPosition() const { return worldPosition; }
    void wrapPosition();
    void updatePosition(const sf::Vector2f& cameraOffset);

    // Weapon system
    void update(float deltaTime, const std::vector<std::unique_ptr<Enemy>>& enemies);
    void attack(const std::vector<std::unique_ptr<Enemy>>& enemies);
    void switchWeapon(int weaponIndex);
    void addWeapon(std::unique_ptr<Weapon> weapon);
    void clearWeapons();
    Weapon* getCurrentWeapon() const;
    int getCurrentWeaponIndex() const { return currentWeaponIndex_; }
    int getWeaponCount() const { return static_cast<int>(weapons_.size()); }

    // Talent system
    bool needsLevelUp() const { return pendingLevelUps_ > 0; }
    void processLevelUp();
    TalentTree& getTalentTree() { return talentTree_; }
    const TalentTree& getTalentTree() const { return talentTree_; }
    void selectTalent(int talentIndex);
    
    // Getters for stats (for talent modifications)
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth_; }
    int getLevel() const { return level; }
    int getExperience() const { return experience; }
    int getGold() const { return gold; }
    float getSpeed() const { return speed; }
    
    // Talent stat modifiers
    float getDamageMultiplier() const { return damageMultiplier_; }
    float getAttackSpeedMultiplier() const { return attackSpeedMultiplier_; }
    float getCritChance() const { return critChance_; }
    float getCritMultiplier() const { return critMultiplier_; }
    float getLifeStealPercent() const { return lifeStealPercent_; }
    float getArmorValue() const { return armorValue_; }
    float getRegenRate() const { return regenRate_; }
    float getExperienceMultiplier() const { return experienceMultiplier_; }
    float getGoldMultiplier() const { return goldMultiplier_; }
    float getDodgeChance() const { return dodgeChance_; }
    
    // Setters for talent modifications
    void setMaxHealth(int maxHealth) { maxHealth_ = maxHealth; }
    void setSpeed(float newSpeed) { speed = newSpeed; }
    void healPlayer(int amount);
    void addDamageMultiplier(float bonus) { damageMultiplier_ += bonus; }
    void addAttackSpeedMultiplier(float bonus) { attackSpeedMultiplier_ += bonus; }
    void addCritChance(float bonus) { critChance_ += bonus; }
    void addCritMultiplier(float bonus) { critMultiplier_ += bonus; }
    void addLifeSteal(float bonus) { lifeStealPercent_ += bonus; }
    void addArmor(float bonus) { armorValue_ += bonus; }
    void addRegenRate(float bonus) { regenRate_ += bonus; }
    void addExperienceMultiplier(float bonus) { experienceMultiplier_ += bonus; }
    void addGoldMultiplier(float bonus) { goldMultiplier_ += bonus; }
    void addDodgeChance(float bonus) { dodgeChance_ += bonus; }
    
    // Helper methods for calculating talent-modified values
    int calculateModifiedDamage(int baseDamage) const;
    float calculateModifiedCooldown(float baseCooldown) const;
    void applyLifeSteal(int damageDealt);

private:
    int health;
    int maxHealth_;
    int level;
    int experience;
    int gold;
    float speed;
    sf::RectangleShape shape;
    sf::Text healthText;
    sf::Font font;
    sf::Vector2f worldPosition;  // World position (independent of camera)

    // Weapon system
    std::vector<std::unique_ptr<Weapon>> weapons_;
    int currentWeaponIndex_;
    bool autoAttack_;
    
    // Talent system
    TalentTree talentTree_;
    int pendingLevelUps_;
    
    // Talent stat modifiers
    float damageMultiplier_;
    float attackSpeedMultiplier_;
    float critChance_;
    float critMultiplier_;
    float lifeStealPercent_;
    float armorValue_;
    float regenRate_;
    float experienceMultiplier_;
    float goldMultiplier_;
    float dodgeChance_;
    
    // Calculate experience needed for next level
    int getExperienceNeeded() const;
}; 