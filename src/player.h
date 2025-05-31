#pragma once
#include <SFML/Graphics.hpp>

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

private:
    int health;
    int level;
    int experience;
    int gold;
    int speed;
    sf::RectangleShape shape;
    sf::Text healthText;
    sf::Font font;
}; 