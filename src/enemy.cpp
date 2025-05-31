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
        
        if (rand() % 100 < 70) {
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

// GruntEnemy implementation
GruntEnemy::GruntEnemy() : Enemy() {
    health = maxHealth = 80;
    speed = 150.0f;
    damage = 10;
    attackRange = 45.0f;
    attackCooldown = 1.2f;
    directionChangeInterval = 1.0f;
    
    initializeShape(sf::Vector2f(40.f, 40.f), sf::Color::Red);
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
    
    initializeShape(sf::Vector2f(25.f, 25.f), sf::Color::Yellow);
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

// BruteEnemy implementation
BruteEnemy::BruteEnemy() : Enemy(), chargeSpeed(400.0f), isCharging(false) {
    health = maxHealth = 150;
    speed = 80.0f;
    damage = 25;
    attackRange = 70.0f;
    attackCooldown = 2.0f;
    directionChangeInterval = 2.0f; // Slower to change direction
    
    initializeShape(sf::Vector2f(60.f, 60.f), sf::Color(139, 69, 19)); // Brown
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
    
    sf::Vector2f direction = player.getPosition() - shape.getPosition();
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
    
    initializeShape(sf::Vector2f(30.f, 30.f), sf::Color::Magenta);
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
        sf::RectangleShape stealthShape = shape;
        sf::Color stealthColor = shape.getFillColor();
        stealthColor.a = 100; // Semi-transparent
        stealthShape.setFillColor(stealthColor);
        window.draw(stealthShape);
    } else {
        Enemy::draw(window);
    }
}

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

// Factory function
std::unique_ptr<Enemy> createRandomEnemy() {
    int typeRoll = rand() % 100;
    
    if (typeRoll < 30) {
        return std::make_unique<GruntEnemy>();
    } else if (typeRoll < 45) {
        return std::make_unique<ScoutEnemy>();
    } else if (typeRoll < 60) {
        return std::make_unique<BruteEnemy>();
    } else if (typeRoll < 75) {
        return std::make_unique<AssassinEnemy>();
    } else if (typeRoll < 85) {
        return std::make_unique<SniperEnemy>();
    } else {
        return std::make_unique<SwarmEnemy>();
    }
}