#pragma once
#include "../enemy.h"

// Special enemy types with unique mechanics
class SniperEnemy : public Enemy {
public:
    SniperEnemy();
    void updateAI(const sf::Vector2f& playerPos, float deltaTime) override;
    void attack(Player& player) override;
private:
    float aimTimer;
    bool isAiming;
    static constexpr float AIM_TIME = 2.0f;
    static constexpr float SNIPE_RANGE = 200.0f;
};

class SwarmEnemy : public Enemy {
public:
    SwarmEnemy();
    void updateAI(const sf::Vector2f& playerPos, float deltaTime) override;
    void move(float deltaTime) override;
private:
    float swarmRadius;
    sf::Vector2f swarmCenter;
    float orbitAngle;
}; 