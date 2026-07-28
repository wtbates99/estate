#pragma once
#include <SFML/Graphics.hpp>

class Player;

class HealthPack {
public:
    HealthPack(const sf::Vector2f& position);
    
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    bool checkCollision(const Player& player) const;
    bool isExpired() const { return lifetime <= 0.0f; }
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    int getHealAmount() const { return healAmount; }
    
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f worldPosition;
    float lifetime; // Health pack disappears after some time
    int healAmount;
    float bobTimer; // For visual bobbing effect
    float bobAmount;
    bool hasTexture;
    sf::RectangleShape fallbackShape; // Fallback if texture fails to load
    
    void loadVisuals();
}; 