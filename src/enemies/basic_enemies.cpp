#include "basic_enemies.h"
#include "../config.h"
#include <cmath>

// GruntEnemy implementation
GruntEnemy::GruntEnemy() : Enemy() {
    health = maxHealth = 80;
    speed = 150.0f;
    damage = 10;
    attackRange = 45.0f;
    attackCooldown = 1.2f;
    directionChangeInterval = 1.0f;
    experienceValue = 15; // Basic grunt gives moderate experience
    
    // Try to load sprite, fallback to colored rectangle if it fails
    if (!loadTexture("assets/textures/enemies/grunt.png")) {
        initializeShape(sf::Vector2f(50.f, 50.f), sf::Color::Red);
    } else {
        sprite.setTexture(texture);
        sprite.setScale(sf::Vector2f(1.0f, 1.0f)); // Moderate scale increase
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        sprite.setPosition(worldPosition);
    }
}

void GruntEnemy::updateAI(const sf::Vector2f& playerPos, float deltaTime) {
    // Grunt has basic AI - just call base implementation
    Enemy::updateAI(playerPos, deltaTime);
}

// ScoutEnemy implementation
ScoutEnemy::ScoutEnemy() : Enemy() {
    health = maxHealth = 40;
    speed = 280.0f;
    damage = 8;
    attackRange = 35.0f;
    attackCooldown = 0.8f;
    directionChangeInterval = 0.5f; // Changes direction more frequently
    experienceValue = 20; // Fast and tricky, gives more experience
    
    // Try to load sprite, fallback to colored rectangle if it fails
    if (!loadTexture("assets/textures/enemies/scout.png")) {
        initializeShape(sf::Vector2f(35.f, 35.f), sf::Color::Yellow);
    } else {
        sprite.setTexture(texture);
        sprite.setScale(sf::Vector2f(0.8f, 0.8f)); // Moderate scale increase
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        sprite.setPosition(worldPosition);
    }
}

void ScoutEnemy::updateAI(const sf::Vector2f& playerPos, float deltaTime) {
    directionChangeTimer += deltaTime;
    
    // Scout is more erratic - 50% chase, 50% random
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

void ScoutEnemy::move(float deltaTime) {
    // Scout moves in a more jittery pattern
    Enemy::move(deltaTime);
} 