#include "special_enemies.h"
#include "../config.h"
#include <cmath>

// SniperEnemy implementation
SniperEnemy::SniperEnemy() : Enemy(), aimTimer(0.0f), isAiming(false) {
    health = maxHealth = 50;
    speed = 100.0f;
    damage = 40; // High damage
    attackRange = SNIPE_RANGE;
    attackCooldown = 4.0f; // Slow but powerful attacks
    directionChangeInterval = 3.0f; // Stays in position longer
    
    initializeShape(sf::Vector2f(35.f, 35.f), sf::Color::Cyan);
}

void SniperEnemy::updateAI(const sf::Vector2f& playerPos, float deltaTime) {
    directionChangeTimer += deltaTime;
    aimTimer += deltaTime;
    
    // Calculate distance to player
    sf::Vector2f direction = playerPos - worldPosition;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    // Sniper tries to maintain distance and aim
    if (distance < SNIPE_RANGE * 0.7f) {
        // Too close - back away
        targetPosition = worldPosition - direction * 0.5f;
        isAiming = false;
        aimTimer = 0.0f;
    } else if (distance > SNIPE_RANGE * 1.2f) {
        // Too far - move closer
        targetPosition = playerPos;
        isAiming = false;
        aimTimer = 0.0f;
    } else {
        // Good range - start aiming
        targetPosition = worldPosition; // Stay in place
        if (aimTimer > AIM_TIME) {
            isAiming = true;
        }
    }
    
    moveTowards(targetPosition, deltaTime);
}

void SniperEnemy::attack(Player& player) {
    if (attackTimer < attackCooldown || !isAiming) return;
    
    sf::Vector2f direction = player.getPosition() - shape.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (distance < SNIPE_RANGE) {
        player.takeDamage(damage);
        attackTimer = 0.0f;
        aimTimer = 0.0f;
        isAiming = false;
        
        // Visual feedback: change color briefly when shooting
        shape.setFillColor(sf::Color::White);
    }
}

// SwarmEnemy implementation
SwarmEnemy::SwarmEnemy() : Enemy(), swarmRadius(50.0f), orbitAngle(0.0f) {
    health = maxHealth = 30;
    speed = 200.0f;
    damage = 6;
    attackRange = 30.0f;
    attackCooldown = 0.7f;
    directionChangeInterval = 0.3f; // Very frequent direction changes
    
    // Random starting orbit angle
    orbitAngle = (rand() % 360) * (3.14159f / 180.0f);
    
    initializeShape(sf::Vector2f(20.f, 20.f), sf::Color(255, 165, 0)); // Orange
}

void SwarmEnemy::updateAI(const sf::Vector2f& playerPos, float deltaTime) {
    directionChangeTimer += deltaTime;
    
    // Update swarm center to be near the player
    swarmCenter = playerPos;
    
    // Orbit around the swarm center
    orbitAngle += deltaTime * 2.0f; // Orbit speed
    
    // Calculate orbit position
    sf::Vector2f orbitOffset(
        cos(orbitAngle) * swarmRadius,
        sin(orbitAngle) * swarmRadius
    );
    
    targetPosition = swarmCenter + orbitOffset;
    
    // Add some randomness to make swarming look more organic
    if (directionChangeTimer >= directionChangeInterval) {
        directionChangeTimer = 0.0f;
        
        float randomAngle = (rand() % 60 - 30) * (3.14159f / 180.0f); // ±30 degrees
        swarmRadius += (rand() % 20 - 10); // Vary radius slightly
        swarmRadius = std::max(30.0f, std::min(swarmRadius, 80.0f)); // Keep within bounds
        
        orbitAngle += randomAngle;
    }
    
    moveTowards(targetPosition, deltaTime);
}

void SwarmEnemy::move(float deltaTime) {
    // Swarm enemies move more erratically
    Enemy::move(deltaTime);
    
    // Add slight bobbing motion
    float bobOffset = sin(orbitAngle * 3.0f) * 2.0f;
    worldPosition.y += bobOffset * deltaTime;
    wrapPosition();
} 