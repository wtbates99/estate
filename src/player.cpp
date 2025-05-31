#include <string>
#include <SFML/Graphics.hpp>
#include "player.h"
#include "config.h"
#include "enemy.h"
#include "weapons/weapon_factory.h"

Player::Player() : 
    health(Config::PLAYER_START_HEALTH), 
    level(1), 
    experience(0), 
    gold(0), 
    speed(Config::PLAYER_SPEED),
    currentWeaponIndex_(0),
    autoAttack_(true) {
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

    // Initialize with default weapons using the factory
    weapons_.push_back(createSword());
    weapons_.push_back(createBow());
    weapons_.push_back(createDagger());
    weapons_.push_back(createCrossbow());
}

void Player::move(float deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        worldPosition.x -= speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        worldPosition.x += speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        worldPosition.y -= speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        worldPosition.y += speed * deltaTime;
    }
    
    // Update shape position to match world position
    shape.setPosition(worldPosition);
}

void Player::update(float deltaTime, const std::vector<std::unique_ptr<Enemy>>& enemies) {
    // Update current weapon
    if (getCurrentWeapon()) {
        getCurrentWeapon()->update(deltaTime);
        
        // Handle ranged weapon projectile updates with enemy collision
        if (getCurrentWeapon()->getType() == Weapon::Type::RANGED) {
            RangedWeapon* rangedWeapon = static_cast<RangedWeapon*>(getCurrentWeapon());
            rangedWeapon->updateProjectiles(deltaTime, enemies);
        }
    }

    // Handle weapon switching
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && weapons_.size() > 0) {
        switchWeapon(0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && weapons_.size() > 1) {
        switchWeapon(1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) && weapons_.size() > 2) {
        switchWeapon(2);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4) && weapons_.size() > 3) {
        switchWeapon(3);
    }

    // Handle manual attack
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        attack(enemies);
    }

    // Auto-attack if enabled and enemies are nearby
    if (autoAttack_ && getCurrentWeapon() && getCurrentWeapon()->canAttack(deltaTime)) {
        // Check if there are enemies in range
        if (getCurrentWeapon()->hasEnemiesInRange(worldPosition, enemies)) {
            attack(enemies);
        }
    }
}

void Player::attack(const std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (getCurrentWeapon()) {
        getCurrentWeapon()->attack(worldPosition, enemies);
    }
}

void Player::switchWeapon(int weaponIndex) {
    if (weaponIndex >= 0 && weaponIndex < static_cast<int>(weapons_.size())) {
        currentWeaponIndex_ = weaponIndex;
    }
}

void Player::addWeapon(std::unique_ptr<Weapon> weapon) {
    weapons_.push_back(std::move(weapon));
}

Weapon* Player::getCurrentWeapon() const {
    if (currentWeaponIndex_ >= 0 && currentWeaponIndex_ < static_cast<int>(weapons_.size())) {
        return weapons_[currentWeaponIndex_].get();
    }
    return nullptr;
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
    
    // Draw current weapon effects
    if (getCurrentWeapon()) {
        getCurrentWeapon()->draw(window, worldPosition);
    }
    
    // Update and draw health text
    healthText.setString("HP: " + std::to_string(health));
    // Position text above player
    sf::FloatRect textBounds = healthText.getLocalBounds();
    healthText.setPosition(
        shape.getPosition().x - textBounds.width / 2,
        shape.getPosition().y - shape.getSize().y / 2 - 30
    );
    window.draw(healthText);

    // Draw weapon info
    if (getCurrentWeapon()) {
        sf::Text weaponText;
        weaponText.setFont(font);
        weaponText.setCharacterSize(16);
        weaponText.setFillColor(sf::Color::Cyan);
        
        std::string weaponInfo = "Weapon: " + std::to_string(currentWeaponIndex_ + 1) + "/" + std::to_string(weapons_.size());
        weaponInfo += " (";
        weaponInfo += (getCurrentWeapon()->getType() == Weapon::Type::MELEE ? "Melee" : "Ranged");
        weaponInfo += ")";
        weaponText.setString(weaponInfo);
        
        // Position weapon text below health
        sf::FloatRect weaponBounds = weaponText.getLocalBounds();
        weaponText.setPosition(
            shape.getPosition().x - weaponBounds.width / 2,
            shape.getPosition().y - shape.getSize().y / 2 - 50
        );
        window.draw(weaponText);
    }
}

void Player::wrapPosition() {
    // Wrap position if player goes out of bounds
    if (worldPosition.x < 0) worldPosition.x = Config::WORLD_WIDTH;
    if (worldPosition.x > Config::WORLD_WIDTH) worldPosition.x = 0;
    if (worldPosition.y < 0) worldPosition.y = Config::WORLD_HEIGHT;
    if (worldPosition.y > Config::WORLD_HEIGHT) worldPosition.y = 1;
}

