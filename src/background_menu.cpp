#include "background_menu.h"
#include "config.h"
#include <iostream>

BackgroundMenu::BackgroundMenu() 
    : selectedIndex(0), menuActive(false), selectedBackground(BackgroundType::FOREST),
      normalColor(sf::Color::White), selectedColor(sf::Color::Yellow), descriptionColor(sf::Color(200, 200, 200)) {
    
    availableBackgrounds = getAvailableBackgrounds();
}

bool BackgroundMenu::initialize() {
    // Load font
    if (!font.loadFromFile("ARIAL.TTF")) {
        std::cerr << "Failed to load font for background menu" << std::endl;
        return false;
    }
    
    // Set up title text
    titleText.setFont(font);
    titleText.setString("Choose Your Background");
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color::Cyan);
    
    // Center title
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition(
        (Config::WINDOW_WIDTH - titleBounds.width) / 2,
        50
    );
    
    // Set up instruction text
    instructionText.setFont(font);
    instructionText.setString("Use UP/DOWN arrows to navigate, ENTER to select, ESC for random");
    instructionText.setCharacterSize(20);
    instructionText.setFillColor(sf::Color::White);
    
    // Center instruction text
    sf::FloatRect instrBounds = instructionText.getLocalBounds();
    instructionText.setPosition(
        (Config::WINDOW_WIDTH - instrBounds.width) / 2,
        Config::WINDOW_HEIGHT - 60
    );
    
    // Set up option texts
    optionTexts.clear();
    descriptionTexts.clear();
    
    float startY = 150;
    float lineHeight = 60;
    
    for (size_t i = 0; i < availableBackgrounds.size(); ++i) {
        // Option text
        sf::Text optionText;
        optionText.setFont(font);
        optionText.setString(availableBackgrounds[i].name);
        optionText.setCharacterSize(32);
        optionText.setFillColor(i == 0 ? selectedColor : normalColor);
        optionText.setPosition(100, startY + i * lineHeight);
        optionTexts.push_back(optionText);
        
        // Description text
        sf::Text descText;
        descText.setFont(font);
        descText.setString(availableBackgrounds[i].description);
        descText.setCharacterSize(18);
        descText.setFillColor(descriptionColor);
        descText.setPosition(120, startY + i * lineHeight + 35);
        descriptionTexts.push_back(descText);
    }
    
    return true;
}

void BackgroundMenu::handleEvent(const sf::Event& event) {
    if (!menuActive) return;
    
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Up:
                selectedIndex = (selectedIndex - 1 + availableBackgrounds.size()) % availableBackgrounds.size();
                break;
                
            case sf::Keyboard::Down:
                selectedIndex = (selectedIndex + 1) % availableBackgrounds.size();
                break;
                
            case sf::Keyboard::Enter:
                selectedBackground = availableBackgrounds[selectedIndex].type;
                menuActive = false;
                break;
                
            case sf::Keyboard::Escape:
                // Select random background (excluding custom)
                selectedBackground = BackgroundType::FOREST; // Default fallback
                for (const auto& bg : availableBackgrounds) {
                    if (bg.type != BackgroundType::CUSTOM) {
                        selectedBackground = bg.type;
                        break;
                    }
                }
                menuActive = false;
                break;
                
            default:
                break;
        }
        
        // Update text colors based on selection
        for (size_t i = 0; i < optionTexts.size(); ++i) {
            optionTexts[i].setFillColor(i == selectedIndex ? selectedColor : normalColor);
        }
    }
}

void BackgroundMenu::update(float deltaTime) {
    // Could add animations or other dynamic effects here
}

void BackgroundMenu::draw(sf::RenderWindow& window) {
    if (!menuActive) return;
    
    // Draw semi-transparent background
    sf::RectangleShape overlay(sf::Vector2f(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(overlay);
    
    // Draw menu elements
    window.draw(titleText);
    window.draw(instructionText);
    
    for (const auto& optionText : optionTexts) {
        window.draw(optionText);
    }
    
    for (const auto& descText : descriptionTexts) {
        window.draw(descText);
    }
} 