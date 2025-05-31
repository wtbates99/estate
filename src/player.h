#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "config.h"

class Player {
public:
    Player();
    void move(float deltaTime);
    void takeDamage(int damage);
    void gainExperience(int exp);
    void addGold(int amount);
    bool isAlive() const;
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
    sf::Vector2f getPosition() const { return shape.getPosition(); }
    sf::Vector2f getWorldPosition() const { return worldPosition; }
    void wrapPosition();
    void updatePosition(const sf::Vector2f& cameraOffset);

private:
    int health;
    int level;
    int experience;
    int gold;
    float speed;
    sf::RectangleShape shape;
    sf::Text healthText;
    sf::Font font;
    sf::Vector2f worldPosition;  // World position (independent of camera)
}; 