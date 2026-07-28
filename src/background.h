#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Background {
private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    bool isLoaded;
    
public:
    Background();
    ~Background() = default;
    
    // Load background texture from file
    bool loadFromFile(const std::string& filename);
    
    // Draw the background with proper camera positioning
    void draw(sf::RenderWindow& window, const sf::Vector2f& cameraCenter);
    
    // Check if background is loaded and ready
    bool isBackgroundLoaded() const { return isLoaded; }
}; 