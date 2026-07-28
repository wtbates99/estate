#include "gold_drop.h"
#include "player.h"
#include "config.h"
#include <cmath>

GoldDrop::GoldDrop(const sf::Vector2f& position, int value) 
    : position_(position), value_(value), active_(true), 
      lifetime_(0.0f), maxLifetime_(30.0f), pulseTimer_(0.0f), baseRadius_(8.0f) {
    
    // Initialize visual shape
    shape_.setRadius(baseRadius_);
    shape_.setFillColor(sf::Color::Yellow);
    shape_.setOutlineColor(sf::Color(255, 215, 0)); // Gold color
    shape_.setOutlineThickness(2.0f);
    shape_.setOrigin(baseRadius_, baseRadius_);
    shape_.setPosition(position_);
    
    // Initialize text
    if (font_.loadFromFile("ARIAL.TTF")) {
        valueText_.setFont(font_);
        valueText_.setString(std::to_string(value_));
        valueText_.setCharacterSize(12);
        valueText_.setFillColor(sf::Color::White);
        valueText_.setStyle(sf::Text::Bold);
        
        // Center the text on the gold drop
        sf::FloatRect textBounds = valueText_.getLocalBounds();
        valueText_.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);
        valueText_.setPosition(position_);
    }
}

void GoldDrop::update(float deltaTime) {
    if (!active_) return;
    
    lifetime_ += deltaTime;
    pulseTimer_ += deltaTime;
    
    // Remove after max lifetime
    if (lifetime_ >= maxLifetime_) {
        active_ = false;
        return;
    }
    
    // Pulsing effect
    float pulseScale = 1.0f + 0.2f * std::sin(pulseTimer_ * 8.0f);
    shape_.setRadius(baseRadius_ * pulseScale);
    shape_.setOrigin(baseRadius_ * pulseScale, baseRadius_ * pulseScale);
    
    // Fade out in last 5 seconds
    if (lifetime_ > maxLifetime_ - 5.0f) {
        float alpha = (maxLifetime_ - lifetime_) / 5.0f;
        sf::Color color = shape_.getFillColor();
        color.a = static_cast<sf::Uint8>(255 * alpha);
        shape_.setFillColor(color);
        
        sf::Color outlineColor = shape_.getOutlineColor();
        outlineColor.a = static_cast<sf::Uint8>(255 * alpha);
        shape_.setOutlineColor(outlineColor);
        
        sf::Color textColor = valueText_.getFillColor();
        textColor.a = static_cast<sf::Uint8>(255 * alpha);
        valueText_.setFillColor(textColor);
    }
}

void GoldDrop::draw(sf::RenderWindow& window) const {
    if (!active_) return;
    
    window.draw(shape_);
    window.draw(valueText_);
}

bool GoldDrop::checkCollision(const Player& player) {
    if (!active_) return false;
    
    sf::Vector2f playerPos = player.getPosition();
    sf::Vector2f goldPos = position_;
    
    float distance = std::sqrt(std::pow(playerPos.x - goldPos.x, 2) + 
                              std::pow(playerPos.y - goldPos.y, 2));
    
    // Collection radius slightly larger than visual radius
    if (distance < baseRadius_ + 25.0f) {
        active_ = false;
        return true;
    }
    
    return false;
}