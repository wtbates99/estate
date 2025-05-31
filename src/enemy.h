#pragma once
#include <SFML/Graphics.hpp>
#include "player.h"

enum class EnemyType {
    GRUNT,      // Basic enemy - balanced stats
    SCOUT,      // Fast but weak
    BRUTE,      // Slow but strong with high attack range
    ASSASSIN    // Fast with short attack range but high damage
};

class Enemy {
public:
    Enemy(EnemyType type = EnemyType::GRUNT);
    void move(float deltaTime);
    void takeDamage(int damage);
    void attack(Player& player);
    bool isAlive() const;
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;
    void updatePosition(const sf::Vector2f& playerPos, const sf::Vector2f& cameraOffset);
    sf::Vector2f getPosition() const { return shape.getPosition(); }
    sf::Vector2f getWorldPosition() const { return worldPosition; }
    EnemyType getType() const { return type; }

private:
    EnemyType type;
    int health;
    int speed;
    int damage;
    float attackRange;
    sf::RectangleShape shape;
    
    // Movement state
    sf::Vector2f targetPosition;
    float directionChangeTimer;
    static constexpr float DIRECTION_CHANGE_INTERVAL = 1.0f; // Change direction every second
    
    // World position (independent of camera)
    sf::Vector2f worldPosition;
    
    // Helper function to initialize stats based on type
    void initializeStats();
}; 