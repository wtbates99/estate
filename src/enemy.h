#pragma once
#include <SFML/Graphics.hpp>
#include "player.h"
#include <memory>
#include <string>

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
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    sf::Vector2f getWorldPosition() const { return worldPosition; }
    void updatePosition(const sf::Vector2f& playerPos, const sf::Vector2f& cameraOffset);
    
    // Health bar methods
    void drawHealthBar(sf::RenderWindow& window) const;
    void setHealthBarVisible(bool visible) { showHealthBar = visible; }
    bool isHealthBarVisible() const { return showHealthBar; }

protected:
    // Protected members that derived classes can access
    int health;
    int maxHealth;
    float speed;
    int damage;
    float attackRange;
    float attackCooldown;
    float attackTimer;
    
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f worldPosition;
    sf::Vector2f targetPosition;
    
    // AI state
    float directionChangeTimer;
    float directionChangeInterval;
    
    // Health bar properties
    bool showHealthBar;
    mutable sf::RectangleShape healthBarBackground;
    mutable sf::RectangleShape healthBarForeground;
    
    // Helper methods for derived classes
    void moveTowards(const sf::Vector2f& target, float deltaTime);
    void moveRandomly(float deltaTime);
    void wrapPosition();
    void initializeSprite(const std::string& texturePath, const sf::Vector2f& scale = sf::Vector2f(1.0f, 1.0f));
    void initializeShape(const sf::Vector2f& size, const sf::Color& color); // Keep for fallback
    
    // Texture loading helper
    bool loadTexture(const std::string& texturePath);
};

// Legacy enum for compatibility (can be removed later)
enum class EnemyType {
    GRUNT, SCOUT, BRUTE, ASSASSIN, SNIPER, SWARM
}; 