#pragma once
#include <SFML/Graphics.hpp>
#include "player.h"

class Enemy {
public:
    Enemy();
    void move(float deltaTime);
    void takeDamage(int damage);
    void attack(Player& player);
    bool isAlive() const;
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;

private:
    int health;
    int speed;
    int damage;
    sf::RectangleShape shape;
}; 