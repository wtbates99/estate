#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "weapon_tier_system.h"

class Player;

class WeaponSelection {
public:
    WeaponSelection();
    
    void draw(sf::RenderWindow& window) const;
    void handleInput(sf::Event& event, Player& player);
    void update(float deltaTime);
    
    bool isActive() const { return active_; }
    void setActive(bool active) { active_ = active; }
    bool isSelectionComplete() const { return selectionComplete_; }
    
private:
    bool active_;
    bool selectionComplete_;
    int selectedWeapon_;
    
    // Visual components
    sf::RectangleShape background_;
    sf::Text titleText_;
    sf::Text instructionText_;
    sf::Text weaponListText_;
    sf::Text weaponDetailsText_;
    sf::Font font_;
    
    // Available basic weapons for selection
    std::vector<WeaponCategory> availableWeapons_;
    
    // Private methods
    void initializeAvailableWeapons();
    void updateWeaponDisplay();
    std::string formatWeaponList() const;
    std::string formatWeaponDetails() const;
    void selectWeapon(Player& player);
};