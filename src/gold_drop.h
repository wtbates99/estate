#pragma once
#include <SFML/Graphics.hpp>

class Player;

class GoldDrop {
public:
    GoldDrop(const sf::Vector2f& position, int value);
    
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    bool checkCollision(const Player& player);
    bool isActive() const { return active_; }
    int getValue() const { return value_; }
    sf::Vector2f getPosition() const { return position_; }
    
private:
    sf::Vector2f position_;
    int value_;
    bool active_;
    float lifetime_;
    float maxLifetime_;
    sf::CircleShape shape_;
    sf::Text valueText_;
    sf::Font font_;
    
    // Visual effects
    float pulseTimer_;
    float baseRadius_;
};