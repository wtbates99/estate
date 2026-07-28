#pragma once
#include "../enemy.h"

// Combat-focused enemy types with advanced behaviors
class BruteEnemy : public Enemy {
public:
    BruteEnemy();
    void updateAI(const sf::Vector2f& playerPos, float deltaTime) override;
    void attack(Player& player) override;
private:
    float chargeSpeed;
    bool isCharging;
};

class AssassinEnemy : public Enemy {
public:
    AssassinEnemy();
    void updateAI(const sf::Vector2f& playerPos, float deltaTime) override;
    void draw(sf::RenderWindow& window) const override;
private:
    float stealthTimer;
    bool isStealthed;
    static constexpr float STEALTH_DURATION = 3.0f;
    static constexpr float STEALTH_COOLDOWN = 8.0f;
}; 