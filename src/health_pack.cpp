#include "health_pack.h"
#include "player.h"
#include <cmath>
#include <iostream>

HealthPack::HealthPack(const sf::Vector2f& position) :
    worldPosition(position),
    lifetime(20.0f), // Health pack lasts for 20 seconds
    healAmount(25), // Heals 25 HP
    bobTimer(0.0f),
    bobAmount(5.0f),
    hasTexture(false) {
    
    std::cout << "Creating health pack at position: (" << position.x << ", " << position.y << ")" << std::endl;
    loadVisuals();
}

void HealthPack::loadVisuals() {
    // Try to load health pack texture
    if (texture.loadFromFile("assets/textures/health_pack.png")) {
        hasTexture = true;
        sprite.setTexture(texture);
        
        // Center the origin
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        sprite.setScale(0.8f, 0.8f); // Scale down a bit
        
        std::cout << "Health pack texture loaded successfully" << std::endl;
    } else {
        // Fallback to a green cross shape
        hasTexture = false;
        fallbackShape.setSize(sf::Vector2f(20.f, 20.f));
        fallbackShape.setFillColor(sf::Color::Green);
        fallbackShape.setOrigin(10.f, 10.f);
        
        std::cout << "Could not load health pack texture, using fallback shape" << std::endl;
    }
    
    // Set initial position
    if (hasTexture) {
        sprite.setPosition(worldPosition);
    } else {
        fallbackShape.setPosition(worldPosition);
    }
}

void HealthPack::update(float deltaTime) {
    // Decrease lifetime
    lifetime -= deltaTime;
    
    // Update bobbing animation
    bobTimer += deltaTime * 3.0f; // Speed of bobbing
    float bobOffset = sin(bobTimer) * bobAmount;
    
    sf::Vector2f bobbingPosition = worldPosition + sf::Vector2f(0.f, bobOffset);
    
    if (hasTexture) {
        sprite.setPosition(bobbingPosition);
        
        // Add a slight fade effect when near expiration
        if (lifetime < 5.0f) {
            sf::Uint8 alpha = static_cast<sf::Uint8>(255 * (lifetime / 5.0f));
            sprite.setColor(sf::Color(255, 255, 255, alpha));
        }
    } else {
        fallbackShape.setPosition(bobbingPosition);
        
        // Add a slight fade effect when near expiration
        if (lifetime < 5.0f) {
            sf::Uint8 alpha = static_cast<sf::Uint8>(255 * (lifetime / 5.0f));
            fallbackShape.setFillColor(sf::Color(0, 255, 0, alpha));
        }
    }
}

void HealthPack::draw(sf::RenderWindow& window) const {
    if (hasTexture) {
        window.draw(sprite);
    } else {
        window.draw(fallbackShape);
        
        // Draw a simple cross pattern for the health pack
        sf::RectangleShape horizontalBar(sf::Vector2f(16.f, 4.f));
        sf::RectangleShape verticalBar(sf::Vector2f(4.f, 16.f));
        
        sf::Vector2f pos = fallbackShape.getPosition();
        
        horizontalBar.setOrigin(8.f, 2.f);
        verticalBar.setOrigin(2.f, 8.f);
        
        horizontalBar.setPosition(pos);
        verticalBar.setPosition(pos);
        
        // Use white for the cross
        sf::Color crossColor = sf::Color::White;
        if (lifetime < 5.0f) {
            sf::Uint8 alpha = static_cast<sf::Uint8>(255 * (lifetime / 5.0f));
            crossColor.a = alpha;
        }
        
        horizontalBar.setFillColor(crossColor);
        verticalBar.setFillColor(crossColor);
        
        window.draw(horizontalBar);
        window.draw(verticalBar);
    }
}

bool HealthPack::checkCollision(const Player& player) const {
    sf::FloatRect playerBounds = player.getBounds();
    
    sf::FloatRect healthPackBounds;
    if (hasTexture) {
        healthPackBounds = sprite.getGlobalBounds();
    } else {
        healthPackBounds = fallbackShape.getGlobalBounds();
    }
    
    return playerBounds.intersects(healthPackBounds);
} 