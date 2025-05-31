#pragma once

#include <SFML/Graphics.hpp>
#include "backgrounds/background_types.h"

class BackgroundMenu {
private:
    sf::Font font;
    sf::Text titleText;
    sf::Text instructionText;
    std::vector<sf::Text> optionTexts;
    std::vector<sf::Text> descriptionTexts;
    
    std::vector<BackgroundInfo> availableBackgrounds;
    int selectedIndex;
    bool menuActive;
    BackgroundType selectedBackground;
    
    // Menu styling
    sf::Color normalColor;
    sf::Color selectedColor;
    sf::Color descriptionColor;
    
public:
    BackgroundMenu();
    
    // Initialize the menu (load font, set up texts)
    bool initialize();
    
    // Handle input events
    void handleEvent(const sf::Event& event);
    
    // Update menu state
    void update(float deltaTime);
    
    // Draw the menu
    void draw(sf::RenderWindow& window);
    
    // Check if menu is active
    bool isActive() const { return menuActive; }
    
    // Get selected background type
    BackgroundType getSelectedBackground() const { return selectedBackground; }
    
    // Start the menu
    void activate() { menuActive = true; }
    
    // Close the menu
    void deactivate() { menuActive = false; }
}; 