#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <vector>
#include "config.h"
#include "weapon.h"

// Forward declarations
class Enemy;

class Player {
public:
    Player();
    void move(float deltaTime);
    void takeDamage(int damage);
    void gainExperience(int exp);
    void addGold(int amount);
    bool isAlive() const;
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
    sf::Vector2f getPosition() const { return shape.getPosition(); }
    sf::Vector2f getWorldPosition() const { return worldPosition; }
    void wrapPosition();
    void updatePosition(const sf::Vector2f& cameraOffset);

    // Weapon system
    void update(float deltaTime, const std::vector<std::unique_ptr<Enemy>>& enemies);
    void attack(const std::vector<std::unique_ptr<Enemy>>& enemies);
    void switchWeapon(int weaponIndex);
    void addWeapon(std::unique_ptr<Weapon> weapon);
    Weapon* getCurrentWeapon() const;
    int getCurrentWeaponIndex() const { return currentWeaponIndex_; }
    int getWeaponCount() const { return static_cast<int>(weapons_.size()); }

private:
    int health;
    int level;
    int experience;
    int gold;
    float speed;
    sf::RectangleShape shape;
    sf::Text healthText;
    sf::Font font;
    sf::Vector2f worldPosition;  // World position (independent of camera)

    // Weapon system
    std::vector<std::unique_ptr<Weapon>> weapons_;
    int currentWeaponIndex_;
    bool autoAttack_;
}; 