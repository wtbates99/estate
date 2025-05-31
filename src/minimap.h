#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "player.h"
#include "enemy.h"

class Minimap {
public:
    static void draw(sf::RenderWindow& window, const Player& player, const std::vector<std::unique_ptr<Enemy>>& enemies);
}; 