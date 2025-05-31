#include "melee_weapons.h"
#include <iostream>
#include <cmath>

// Sword implementation
Sword::Sword() : MeleeWeapon(25, 0.8f, 60.0f, 0.3f), textureLoaded_(false) {
    loadAssets();
}

void Sword::loadAssets() {
    if (weaponTexture_.loadFromFile("assets/weapons/sword.png")) {
        weaponSprite_.setTexture(weaponTexture_);
        // Center the sprite origin
        sf::FloatRect bounds = weaponSprite_.getLocalBounds();
        weaponSprite_.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
        textureLoaded_ = true;
    } else {
        std::cout << "Warning: Could not load sword.png asset\n";
        textureLoaded_ = false;
    }
}

void Sword::draw(sf::RenderWindow& window, const sf::Vector2f& playerPos) const {
    if (textureLoaded_ && isSwinging_) {
        // Make a mutable copy of the sprite for drawing
        sf::Sprite drawSprite = weaponSprite_;
        
        // Position weapon relative to player
        sf::Vector2f weaponOffset = swingDirection_ * 30.0f; // Offset from player
        drawSprite.setPosition(playerPos + weaponOffset);
        
        // Rotate weapon based on swing direction
        float angle = std::atan2(swingDirection_.y, swingDirection_.x) * 180.0f / 3.14159f;
        drawSprite.setRotation(angle);
        
        // Apply swing animation scaling
        float swingProgress = (swingDuration_ - swingTimer_) / swingDuration_;
        float scale = 0.8f + 0.4f * swingProgress;
        drawSprite.setScale(scale, scale);
        
        window.draw(drawSprite);
    }
    
    // Call parent draw for swing arc effect
    MeleeWeapon::draw(window, playerPos);
}

// Dagger implementation
Dagger::Dagger() : MeleeWeapon(15, 0.5f, 45.0f, 0.2f), textureLoaded_(false) {
    loadAssets();
}

void Dagger::loadAssets() {
    if (weaponTexture_.loadFromFile("assets/weapons/dagger.png")) {
        weaponSprite_.setTexture(weaponTexture_);
        // Center the sprite origin
        sf::FloatRect bounds = weaponSprite_.getLocalBounds();
        weaponSprite_.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
        textureLoaded_ = true;
    } else {
        std::cout << "Warning: Could not load dagger.png asset\n";
        textureLoaded_ = false;
    }
}

void Dagger::draw(sf::RenderWindow& window, const sf::Vector2f& playerPos) const {
    if (textureLoaded_ && isSwinging_) {
        // Make a mutable copy of the sprite for drawing
        sf::Sprite drawSprite = weaponSprite_;
        
        // Position weapon relative to player
        sf::Vector2f weaponOffset = swingDirection_ * 25.0f; // Closer to player than sword
        drawSprite.setPosition(playerPos + weaponOffset);
        
        // Rotate weapon based on swing direction
        float angle = std::atan2(swingDirection_.y, swingDirection_.x) * 180.0f / 3.14159f;
        drawSprite.setRotation(angle);
        
        // Apply swing animation scaling (faster animation for dagger)
        float swingProgress = (swingDuration_ - swingTimer_) / swingDuration_;
        float scale = 0.9f + 0.3f * swingProgress;
        drawSprite.setScale(scale, scale);
        
        window.draw(drawSprite);
    }
    
    // Call parent draw for swing arc effect
    MeleeWeapon::draw(window, playerPos);
} 