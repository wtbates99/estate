#include <string>
#include <SFML/Graphics.hpp>
#include "player.h"
#include "config.h"

Player::Player() : 
    health(Config::PLAYER_START_HEALTH), 
    level(1), 
    experience(0), 
    gold(0), 
    speed(Config::PLAYER_SPEED) {
    shape.setFillColor(sf::Color::Green);
    shape.setSize(sf::Vector2f(50.f, 50.f));
    shape.setOrigin(25.f, 25.f);
    
    // Initialize world position
    worldPosition = sf::Vector2f(Config::WORLD_WIDTH/2, Config::WORLD_HEIGHT/2);
    shape.setPosition(worldPosition);
    
    // Initialize font and text
    if (!font.loadFromFile("ARIAL.TTF")) {
        // Handle font loading error
    }
    healthText.setFont(font);
    healthText.setCharacterSize(20);
    healthText.setFillColor(sf::Color::White);
}

void Player::move(float deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        worldPosition.x -= speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        worldPosition.x += speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        worldPosition.y -= speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        worldPosition.y += speed * deltaTime;
    }
    
    // Update shape position to match world position
    shape.setPosition(worldPosition);
}

void Player::updatePosition(const sf::Vector2f& cameraOffset) {
    // Update the shape's position relative to the camera
    shape.setPosition(worldPosition - cameraOffset);
}

void Player::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void Player::gainExperience(int exp) {
    experience += exp;
    // Simple level up system
    if (experience >= level * 102) {
        level++;
        experience = 0;
    }
}

void Player::addGold(int amount) {
    gold += amount;
}

bool Player::isAlive() const {
    return health > 0;
}

void Player::draw(sf::RenderWindow& window) {
    // Draw the player shape
    window.draw(shape);
    
    // Update and draw health text
    healthText.setString("HP: " + std::to_string(health));
    // Position text above player
    sf::FloatRect textBounds = healthText.getLocalBounds();
    healthText.setPosition(
        shape.getPosition().x - textBounds.width / 2,
        shape.getPosition().y - shape.getSize().y / 2 - 30
    );
    window.draw(healthText);
}

void Player::wrapPosition() {
    // Wrap position if player goes out of bounds
    if (worldPosition.x < 0) worldPosition.x = Config::WORLD_WIDTH;
    if (worldPosition.x > Config::WORLD_WIDTH) worldPosition.x = 0;
    if (worldPosition.y < 0) worldPosition.y = Config::WORLD_HEIGHT;
    if (worldPosition.y > Config::WORLD_HEIGHT) worldPosition.y = 0;
}

