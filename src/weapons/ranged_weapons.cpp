#include "ranged_weapons.h"
#include <iostream>
#include <cmath>

// Bow implementation
Bow::Bow() : RangedWeapon(20, 1.0f, 250.0f, 350.0f), textureLoaded_(false) {
    loadAssets();
}

void Bow::loadAssets() {
    if (weaponTexture_.loadFromFile("assets/weapons/bow.png")) {
        weaponSprite_.setTexture(weaponTexture_);
        // Center the sprite origin
        sf::FloatRect bounds = weaponSprite_.getLocalBounds();
        weaponSprite_.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
        textureLoaded_ = true;
    } else {
        std::cout << "Warning: Could not load bow.png asset\n";
        textureLoaded_ = false;
    }
}

void Bow::draw(sf::RenderWindow& window, const sf::Vector2f& playerPos) const {
    if (textureLoaded_) {
        // Make a mutable copy of the sprite for drawing
        sf::Sprite drawSprite = weaponSprite_;
        
        // Position weapon next to player
        drawSprite.setPosition(playerPos + sf::Vector2f(15.0f, -10.0f));
        
        // Scale based on cooldown (draw animation)
        float cooldownProgress = 1.0f - (cooldownTimer_ / cooldown_);
        float scale = 0.8f + 0.2f * cooldownProgress;
        drawSprite.setScale(scale, scale);
        
        window.draw(drawSprite);
    }
    
    // Call parent draw for projectiles
    RangedWeapon::draw(window, playerPos);
}

// Crossbow implementation
Crossbow::Crossbow() : RangedWeapon(35, 1.5f, 300.0f, 500.0f), textureLoaded_(false) {
    loadAssets();
}

void Crossbow::loadAssets() {
    if (weaponTexture_.loadFromFile("assets/weapons/crossbow.png")) {
        weaponSprite_.setTexture(weaponTexture_);
        // Center the sprite origin
        sf::FloatRect bounds = weaponSprite_.getLocalBounds();
        weaponSprite_.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
        textureLoaded_ = true;
    } else {
        std::cout << "Warning: Could not load crossbow.png asset\n";
        textureLoaded_ = false;
    }
}

void Crossbow::draw(sf::RenderWindow& window, const sf::Vector2f& playerPos) const {
    if (textureLoaded_) {
        // Make a mutable copy of the sprite for drawing
        sf::Sprite drawSprite = weaponSprite_;
        
        // Position weapon next to player
        drawSprite.setPosition(playerPos + sf::Vector2f(20.0f, -5.0f));
        
        // Scale based on cooldown (reload animation)
        float cooldownProgress = 1.0f - (cooldownTimer_ / cooldown_);
        float scale = 0.9f + 0.1f * cooldownProgress;
        drawSprite.setScale(scale, scale);
        
        // Slight rotation during reload
        float rotation = (1.0f - cooldownProgress) * 5.0f;
        drawSprite.setRotation(rotation);
        
        window.draw(drawSprite);
    }
    
    // Call parent draw for projectiles
    RangedWeapon::draw(window, playerPos);
} 