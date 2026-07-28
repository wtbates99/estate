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
    experienceValue = 30; // Dangerous ranged enemy, good experience
    
    // Try to load sprite, fallback to colored rectangle if it fails
    if (!loadTexture("assets/textures/enemies/sniper.png")) {
        initializeShape(sf::Vector2f(50.f, 50.f), sf::Color::Cyan);
    } else {
        sprite.setTexture(texture);
        sprite.setScale(sf::Vector2f(1.1f, 1.1f)); // Moderate scale increase for sniper
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        sprite.setPosition(worldPosition);
    }
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
    
    sf::Vector2f direction = player.getPosition() - sprite.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (distance < SNIPE_RANGE) {
        player.takeDamage(damage);
        attackTimer = 0.0f;
        aimTimer = 0.0f;
        isAiming = false;
        
        // Visual feedback: change color briefly when shooting
        sprite.setColor(sf::Color::White);
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
    experienceValue = 10; // Weak individually but comes in groups
    
    // Random starting orbit angle
    orbitAngle = (rand() % 360) * (3.14159f / 180.0f);
    
    // Try to load sprite, fallback to colored rectangle if it fails
    if (!loadTexture("assets/textures/enemies/swarm.png")) {
        initializeShape(sf::Vector2f(30.f, 30.f), sf::Color(255, 165, 0)); // Orange
    } else {
        sprite.setTexture(texture);
        sprite.setScale(sf::Vector2f(0.7f, 0.7f)); // Moderate scale increase for swarm
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        sprite.setPosition(worldPosition);
    }
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