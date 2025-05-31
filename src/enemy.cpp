#include "enemy.h"
#include <string>
#include <SFML/Graphics.hpp>
#include "config.h"

Enemy::Enemy() : 
    health(Config::ENEMY_START_HEALTH), 
    speed(Config::ENEMY_SPEED), 
    damage(Config::ENEMY_DAMAGE) {
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(200.f, 200.f);
    shape.setSize(sf::Vector2f(100.f, 100.f));
    shape.setOrigin(50.f, 50.f);
}

void Enemy::move(float deltaTime) {
    shape.move(sf::Vector2f(speed * deltaTime, 0.f));
}

void Enemy::takeDamage(int damage) {
    health -= damage;
}

void Enemy::attack(Player& player) {
    player.takeDamage(damage);
}

bool Enemy::isAlive() const {
    return health > 0;
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Enemy::getBounds() const {
    return shape.getGlobalBounds();
}