#pragma once
#include "../enemy.h"

// Basic enemy types with simple behaviors
class GruntEnemy : public Enemy {
public:
    GruntEnemy();
    void updateAI(const sf::Vector2f& playerPos, float deltaTime) override;
};

class ScoutEnemy : public Enemy {
public:
    ScoutEnemy();
    void updateAI(const sf::Vector2f& playerPos, float deltaTime) override;
    void move(float deltaTime) override;
}; 