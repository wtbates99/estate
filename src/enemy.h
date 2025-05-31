#pragma once
#include <SFML/Graphics.hpp>
#include "player.h"

class Enemy {
public:
    Enemy();
    void move(float deltaTime);
    void takeDamage(int damage);
    void attack(Player& player);
    bool isAlive() const;
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    void updatePosition(const sf::Vector2f& playerPos, const sf::Vector2f& cameraOffset);
    sf::Vector2f getPosition() const { return shape.getPosition(); }
    sf::Vector2f getWorldPosition() const { return worldPosition; }

private:
    int health;
    int speed;
    int damage;
    sf::RectangleShape shape;
    
    // Movement state
    sf::Vector2f targetPosition;
    float directionChangeTimer;
    static constexpr float DIRECTION_CHANGE_INTERVAL = 1.0f; // Change direction every second
    
    // World position (independent of camera)
    sf::Vector2f worldPosition;
}; 