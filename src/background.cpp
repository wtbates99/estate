#include "background.h"
#include "config.h"
#include <iostream>

Background::Background() : isLoaded(false) {
    // Set texture to repeat for potential wrapping effects
    backgroundTexture.setRepeated(false);
}

bool Background::loadFromFile(const std::string& filename) {
    if (!backgroundTexture.loadFromFile(filename)) {
        std::cerr << "Failed to load background texture: " << filename << std::endl;
        isLoaded = false;
        return false;
    }
    
    // Set up the sprite
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setPosition(0, 0);
    
    // Get texture size to verify it matches our world size
    sf::Vector2u textureSize = backgroundTexture.getSize();
    
    // If the texture doesn't match world size, we could scale it
    if (textureSize.x != Config::WORLD_WIDTH || textureSize.y != Config::WORLD_HEIGHT) {
        float scaleX = Config::WORLD_WIDTH / static_cast<float>(textureSize.x);
        float scaleY = Config::WORLD_HEIGHT / static_cast<float>(textureSize.y);
        backgroundSprite.setScale(scaleX, scaleY);
    }
    
    isLoaded = true;
    return true;
}

void Background::draw(sf::RenderWindow& window, const sf::Vector2f& cameraCenter) {
    if (!isLoaded) {
        return;
    }
    
    // The background should always be positioned relative to the world coordinates
    // Since our background matches the world size exactly, we just position it at (0,0)
    backgroundSprite.setPosition(0, 0);
    
    // Draw the background
    window.draw(backgroundSprite);
} 
