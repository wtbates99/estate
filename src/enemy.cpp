#include "enemy.h"
#include <string>
#include <SFML/Graphics.hpp>
#include "config.h"
#include <cmath>

Enemy::Enemy() : 
    health(Config::ENEMY_START_HEALTH), 
    speed(Config::ENEMY_SPEED), 
    damage(Config::ENEMY_DAMAGE),
    directionChangeTimer(0.0f) {
    shape.setFillColor(sf::Color::Red);
    shape.setSize(sf::Vector2f(100.f, 100.f));
    shape.setOrigin(50.f, 50.f);
    
    // Initialize world position randomly in the world
    worldPosition = sf::Vector2f(
        static_cast<float>(rand() % static_cast<int>(Config::WORLD_WIDTH)),
        static_cast<float>(rand() % static_cast<int>(Config::WORLD_HEIGHT))
    );
    shape.setPosition(worldPosition);
    
    // Set initial target position
    targetPosition = worldPosition;
}

void Enemy::move(float deltaTime) {
    directionChangeTimer += deltaTime;
    
    // Only change direction periodically
    if (directionChangeTimer >= DIRECTION_CHANGE_INTERVAL) {
        directionChangeTimer = 0.0f;
        
        // 70% chance to move towards player, 30% chance to move randomly
        if (rand() % 100 < 70) {
            // Move towards player (targetPosition will be updated in updatePosition)
            return;
        } else {
            // Random movement within world bounds
            float angle = (rand() % 360) * (3.14159f / 180.0f); // Random angle in radians
            float distance = Config::WORLD_WIDTH * 0.1f; // 10% of world width
            
            targetPosition = worldPosition + sf::Vector2f(
                cos(angle) * distance,
                sin(angle) * distance
            );
            
            // Ensure target position stays within world bounds
            targetPosition.x = std::max(0.0f, std::min(targetPosition.x, Config::WORLD_WIDTH));
            targetPosition.y = std::max(0.0f, std::min(targetPosition.y, Config::WORLD_HEIGHT));
        }
    }
    
    // Move towards target position
    sf::Vector2f direction = targetPosition - worldPosition;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (length > 0.1f) {
        direction /= length; // Normalize
        worldPosition += direction * static_cast<float>(speed) * deltaTime;
        
        // Wrap position if enemy goes out of bounds
        if (worldPosition.x < 0) worldPosition.x = Config::WORLD_WIDTH;
        if (worldPosition.x > Config::WORLD_WIDTH) worldPosition.x = 0;
        if (worldPosition.y < 0) worldPosition.y = Config::WORLD_HEIGHT;
        if (worldPosition.y > Config::WORLD_HEIGHT) worldPosition.y = 0;
    }
    
    // Update shape position to match world position
    shape.setPosition(worldPosition);
}

void Enemy::updatePosition(const sf::Vector2f& playerPos, const sf::Vector2f& cameraOffset) {
    // Update target position to be the player's position for AI targeting
    targetPosition = playerPos;
}

void Enemy::takeDamage(int damage) {
    health -= damage;
}

void Enemy::attack(Player& player) {
    // Only attack if we're close enough to the player
    sf::Vector2f direction = player.getPosition() - shape.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (distance < 50.0f) { // Attack range of 50 pixels
        player.takeDamage(damage);
    }
}

bool Enemy::isAlive() const {
    return health > 0;
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Enemy::getBounds() const {
    return shape.getGlobalBounds();
}