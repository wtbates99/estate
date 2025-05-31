#pragma once
#include <SFML/Graphics.hpp>
#include "player.h"
#include <memory>

// Base Enemy class
class Enemy {
public:
    Enemy();
    virtual ~Enemy() = default;
    
    // Virtual methods that can be overridden by derived classes
    virtual void move(float deltaTime);
    virtual void updateAI(const sf::Vector2f& playerPos, float deltaTime);
    virtual void attack(Player& player);
    virtual void draw(sf::RenderWindow& window) const;
    
    // Common methods for all enemies
    void takeDamage(int damage);
    bool isAlive() const;
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const { return shape.getPosition(); }
    sf::Vector2f getWorldPosition() const { return worldPosition; }
    void updatePosition(const sf::Vector2f& playerPos, const sf::Vector2f& cameraOffset);

protected:
    // Protected members that derived classes can access
    int health;
    int maxHealth;
    float speed;
    int damage;
    float attackRange;
    float attackCooldown;
    float attackTimer;
    
    sf::RectangleShape shape;
    sf::Vector2f worldPosition;
    sf::Vector2f targetPosition;
    
    // AI state
    float directionChangeTimer;
    float directionChangeInterval;
    
    // Helper methods for derived classes
    void moveTowards(const sf::Vector2f& target, float deltaTime);
    void moveRandomly(float deltaTime);
    void wrapPosition();
    void initializeShape(const sf::Vector2f& size, const sf::Color& color);
};

// Specific enemy types
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

// Example of how easy it is to add new enemy types
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

// Factory function for creating enemies
std::unique_ptr<Enemy> createRandomEnemy();

// Legacy enum for compatibility (can be removed later)
enum class EnemyType {
    GRUNT, SCOUT, BRUTE, ASSASSIN, SNIPER, SWARM
}; 