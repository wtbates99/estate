#include "enemy.h"
#include <string>
#include <SFML/Graphics.hpp>
#include "config.h"
#include <cmath>
#include <random>
#include <iostream>

// Base Enemy class implementation
Enemy::Enemy() : 
    health(100),
    maxHealth(100),
    speed(150.0f),
    damage(10),
    attackRange(45.0f),
    attackCooldown(1.0f),
    attackTimer(0.0f),
    experienceValue(25), // Default experience value
    directionChangeTimer(0.0f),
    directionChangeInterval(1.0f),
    showHealthBar(true) {
    
    // Initialize world position randomly
    worldPosition = sf::Vector2f(
        static_cast<float>(rand() % static_cast<int>(Config::WORLD_WIDTH)),
        static_cast<float>(rand() % static_cast<int>(Config::WORLD_HEIGHT))
    );
    
    targetPosition = worldPosition;
    
    // Initialize health bar shapes
    healthBarBackground.setSize(sf::Vector2f(40.0f, 6.0f));
    healthBarBackground.setFillColor(sf::Color(50, 50, 50, 200));
    healthBarBackground.setOutlineThickness(1.0f);
    healthBarBackground.setOutlineColor(sf::Color::Black);
    
    healthBarForeground.setSize(sf::Vector2f(38.0f, 4.0f));
    healthBarForeground.setFillColor(sf::Color::Green);
}

bool Enemy::loadTexture(const std::string& texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
        return false;
    }
    return true;
}

void Enemy::initializeSprite(const std::string& texturePath, const sf::Vector2f& scale) {
    if (loadTexture(texturePath)) {
        sprite.setTexture(texture);
        sprite.setScale(scale);
        
        // Center the origin
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        sprite.setPosition(worldPosition);
    } else {
        // Fallback to colored rectangle if texture loading fails
        std::cerr << "Falling back to colored rectangle for enemy" << std::endl;
        // This will be handled by individual enemy types
    }
}

void Enemy::initializeShape(const sf::Vector2f& size, const sf::Color& color) {
    // Keep this method for fallback support - convert to sprite-like behavior
    // Create a simple colored texture programmatically
    sf::Image image;
    image.create(static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y), color);
    
    if (texture.loadFromImage(image)) {
        sprite.setTexture(texture);
        sprite.setOrigin(size.x / 2.f, size.y / 2.f);
        sprite.setPosition(worldPosition);
    }
}

void Enemy::move(float deltaTime) {
    attackTimer += deltaTime;
    
    // Update sprite position to match world position
    sprite.setPosition(worldPosition);
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
    
    sf::Vector2f direction = player.getPosition() - sprite.getPosition();
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
    window.draw(sprite);
    
    // Draw health bar if enabled and enemy is damaged
    if (showHealthBar && health < maxHealth) {
        drawHealthBar(window);
    }
}

void Enemy::drawHealthBar(sf::RenderWindow& window) const {
    // Position health bar above the enemy
    sf::Vector2f enemyPos = sprite.getPosition();
    sf::FloatRect bounds = sprite.getGlobalBounds();
    
    sf::Vector2f healthBarPos(
        enemyPos.x - healthBarBackground.getSize().x / 2.0f,
        enemyPos.y - bounds.height / 2.0f - 15.0f
    );
    
    // Draw background
    healthBarBackground.setPosition(healthBarPos);
    window.draw(healthBarBackground);
    
    // Calculate health percentage and determine color
    float healthPercentage = static_cast<float>(health) / static_cast<float>(maxHealth);
    
    // Color based on health percentage
    sf::Color healthColor;
    if (healthPercentage > 0.6f) {
        healthColor = sf::Color::Green;
    } else if (healthPercentage > 0.3f) {
        healthColor = sf::Color::Yellow;
    } else {
        healthColor = sf::Color::Red;
    }
    
    // Draw foreground (health bar)
    healthBarForeground.setFillColor(healthColor);
    healthBarForeground.setSize(sf::Vector2f(38.0f * healthPercentage, 4.0f));
    healthBarForeground.setPosition(healthBarPos.x + 1.0f, healthBarPos.y + 1.0f);
    window.draw(healthBarForeground);
}

sf::FloatRect Enemy::getBounds() const {
    return sprite.getGlobalBounds();
}

bool Enemy::shouldDropHealthPack(const Player& player) {
    // Calculate health percentage (0.0 to 1.0)
    float healthPercentage = static_cast<float>(player.getHealth()) / static_cast<float>(player.getMaxHealth());
    
    // Base drop chance when at full health: 5%
    float baseDropChance = 0.05f; // 5% at full health
    float maxDropChance = 0.50f;  // 50% at very low health
    
    // Invert health percentage so lower health = higher chance
    float lowHealthFactor = 1.0f - healthPercentage;
    
    // Calculate drop chance: more drop chance as health gets lower
    float dropChance = baseDropChance + (maxDropChance - baseDropChance) * lowHealthFactor * lowHealthFactor;
    
    // Generate random number between 0 and 1
    float randomValue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    
    return randomValue < dropChance;
}