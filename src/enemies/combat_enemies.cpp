#include "combat_enemies.h"
#include "../config.h"
#include <cmath>

// BruteEnemy implementation
BruteEnemy::BruteEnemy() : Enemy(), chargeSpeed(400.0f), isCharging(false) {
    health = maxHealth = 150;
    speed = 80.0f;
    damage = 25;
    attackRange = 70.0f;
    attackCooldown = 2.0f;
    directionChangeInterval = 2.0f; // Slower to change direction
    experienceValue = 40; // Tough enemy gives good experience
    
    // Try to load sprite, fallback to colored rectangle if it fails
    if (!loadTexture("assets/textures/enemies/brute.png")) {
        initializeShape(sf::Vector2f(80.f, 80.f), sf::Color(139, 69, 19)); // Brown
    } else {
        sprite.setTexture(texture);
        sprite.setScale(sf::Vector2f(1.5f, 1.5f)); // Moderate scale increase for brute
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        sprite.setPosition(worldPosition);
    }
}

void BruteEnemy::updateAI(const sf::Vector2f& playerPos, float deltaTime) {
    directionChangeTimer += deltaTime;
    
    // Calculate distance to player
    sf::Vector2f direction = playerPos - worldPosition;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    // If close enough, charge at the player
    if (distance < 200.0f && !isCharging) {
        isCharging = true;
        targetPosition = playerPos;
    } else if (distance > 300.0f) {
        isCharging = false;
    }
    
    if (directionChangeTimer >= directionChangeInterval) {
        directionChangeTimer = 0.0f;
        
        if (!isCharging) {
            // Normal movement pattern
            if (rand() % 100 < 80) {
                targetPosition = playerPos;
            } else {
                moveRandomly(deltaTime);
            }
        }
    }
    
    // Use charge speed if charging
    float currentSpeed = isCharging ? chargeSpeed : speed;
    sf::Vector2f dir = targetPosition - worldPosition;
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    
    if (length > 0.1f) {
        dir /= length;
        worldPosition += dir * currentSpeed * deltaTime;
        wrapPosition();
    }
}

void BruteEnemy::attack(Player& player) {
    // Brute does more damage when charging
    if (attackTimer < attackCooldown) return;
    
    sf::Vector2f direction = player.getPosition() - sprite.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (distance < attackRange) {
        int actualDamage = isCharging ? damage * 1.5f : damage;
        player.takeDamage(actualDamage);
        attackTimer = 0.0f;
    }
}

// AssassinEnemy implementation
AssassinEnemy::AssassinEnemy() : Enemy(), stealthTimer(0.0f), isStealthed(false) {
    health = maxHealth = 60;
    speed = 220.0f;
    damage = 20;
    attackRange = 25.0f;
    attackCooldown = 0.6f;
    directionChangeInterval = 0.8f;
    experienceValue = 35; // Stealthy and dangerous, good experience
    
    // Try to load sprite, fallback to colored rectangle if it fails
    if (!loadTexture("assets/textures/enemies/assassin.png")) {
        initializeShape(sf::Vector2f(45.f, 45.f), sf::Color::Magenta);
    } else {
        sprite.setTexture(texture);
        sprite.setScale(sf::Vector2f(0.9f, 0.9f)); // Moderate scale increase for assassin
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        sprite.setPosition(worldPosition);
    }
}

void AssassinEnemy::updateAI(const sf::Vector2f& playerPos, float deltaTime) {
    stealthTimer += deltaTime;
    directionChangeTimer += deltaTime;
    
    // Handle stealth mechanics
    if (!isStealthed && stealthTimer > STEALTH_COOLDOWN) {
        isStealthed = true;
        stealthTimer = 0.0f;
    } else if (isStealthed && stealthTimer > STEALTH_DURATION) {
        isStealthed = false;
        stealthTimer = 0.0f;
    }
    
    // Assassin tries to get close then attack
    sf::Vector2f direction = playerPos - worldPosition;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (distance < 100.0f) {
        // Close to player - move directly towards them
        targetPosition = playerPos;
    } else if (directionChangeTimer >= directionChangeInterval) {
        directionChangeTimer = 0.0f;
        
        // Far from player - mix of tracking and flanking
        if (rand() % 100 < 60) {
            targetPosition = playerPos;
        } else {
            // Try to flank the player
            float angle = atan2(direction.y, direction.x) + (rand() % 2 ? 1.5f : -1.5f);
            float flankDistance = 80.0f;
            targetPosition = playerPos + sf::Vector2f(
                cos(angle) * flankDistance,
                sin(angle) * flankDistance
            );
        }
    }
    
    moveTowards(targetPosition, deltaTime);
}

void AssassinEnemy::draw(sf::RenderWindow& window) const {
    if (isStealthed) {
        // Draw semi-transparent when stealthed
        sf::Sprite stealthSprite = sprite;
        sf::Color stealthColor = sprite.getColor();
        stealthColor.a = 100; // Semi-transparent
        stealthSprite.setColor(stealthColor);
        window.draw(stealthSprite);
    } else {
        Enemy::draw(window);
    }
} 