#include "enemy.h"
#include <string>
#include <SFML/Graphics.hpp>
#include "config.h"
#include <cmath>
#include <random>

// Base Enemy class implementation
Enemy::Enemy() : 
    health(100),
    maxHealth(100),
    speed(150.0f),
    damage(10),
    attackRange(45.0f),
    attackCooldown(1.0f),
    attackTimer(0.0f),
    directionChangeTimer(0.0f),
    directionChangeInterval(1.0f) {
    
    // Initialize world position randomly
    worldPosition = sf::Vector2f(
        static_cast<float>(rand() % static_cast<int>(Config::WORLD_WIDTH)),
        static_cast<float>(rand() % static_cast<int>(Config::WORLD_HEIGHT))
    );
    
    targetPosition = worldPosition;
}

void Enemy::initializeShape(const sf::Vector2f& size, const sf::Color& color) {
    shape.setFillColor(color);
    shape.setSize(size);
    shape.setOrigin(size.x / 2.f, size.y / 2.f);
    shape.setPosition(worldPosition);
}

void Enemy::move(float deltaTime) {
    attackTimer += deltaTime;
    
    // Update shape position to match world position
    shape.setPosition(worldPosition);
}

void Enemy::updateAI(const sf::Vector2f& playerPos, float deltaTime) {
    directionChangeTimer += deltaTime;
    
    // Default AI: move towards player most of the time
    if (directionChangeTimer >= directionChangeInterval) {
        directionChangeTimer = 0.0f;
        
        if (rand() % 100 < 50) {
            targetPosition = playerPos;
        } else {
            moveRandomly(deltaTime);
        }
    }
    
    moveTowards(targetPosition, deltaTime);
}

void Enemy::moveTowards(const sf::Vector2f& target, float deltaTime) {
    sf::Vector2f direction = target - worldPosition;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (length > 0.1f) {
        direction /= length; // Normalize
        worldPosition += direction * speed * deltaTime;
        wrapPosition();
    }
}

void Enemy::moveRandomly(float deltaTime) {
    float angle = (rand() % 360) * (3.14159f / 180.0f);
    float distance = Config::WORLD_WIDTH * 0.1f;
    
    targetPosition = worldPosition + sf::Vector2f(
        cos(angle) * distance,
        sin(angle) * distance
    );
    
    // Keep within bounds
    targetPosition.x = std::max(0.0f, std::min(targetPosition.x, Config::WORLD_WIDTH));
    targetPosition.y = std::max(0.0f, std::min(targetPosition.y, Config::WORLD_HEIGHT));
}

void Enemy::wrapPosition() {
    if (worldPosition.x < 0) worldPosition.x = Config::WORLD_WIDTH;
    if (worldPosition.x > Config::WORLD_WIDTH) worldPosition.x = 0;
    if (worldPosition.y < 0) worldPosition.y = Config::WORLD_HEIGHT;
    if (worldPosition.y > Config::WORLD_HEIGHT) worldPosition.y = 0;
}

void Enemy::updatePosition(const sf::Vector2f& playerPos, const sf::Vector2f& cameraOffset) {
    // This method can be called to update AI with player position
    // Individual enemies will override updateAI for specific behaviors
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void Enemy::attack(Player& player) {
    if (attackTimer < attackCooldown) return;
    
    sf::Vector2f direction = player.getPosition() - shape.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (distance < attackRange) {
        player.takeDamage(damage);
        attackTimer = 0.0f;
    }
}

bool Enemy::isAlive() const {
    return health > 0;
}

void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

sf::FloatRect Enemy::getBounds() const {
    return shape.getGlobalBounds();
}