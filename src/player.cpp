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
    shape.setPosition(400.f, 300.f);
    shape.setSize(sf::Vector2f(50.f, 50.f));
    shape.setOrigin(25.f, 25.f);
    
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
            shape.move(sf::Vector2f(-speed * deltaTime, 0.f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            shape.move(sf::Vector2f(speed * deltaTime, 0.f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            shape.move(sf::Vector2f(0.f, -speed * deltaTime));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            shape.move(sf::Vector2f(0.f, speed * deltaTime));
        }
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

