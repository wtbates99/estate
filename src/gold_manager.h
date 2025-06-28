#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "gold_drop.h"

class Player;

class GoldManager {
public:
    GoldManager();
    
    void update(float deltaTime, Player& player);
    void draw(sf::RenderWindow& window) const;
    void spawnGold(const sf::Vector2f& position, int value);
    void clear();
    
    // Get count for debugging/UI
    size_t getGoldDropCount() const { return goldDrops_.size(); }
    
private:
    std::vector<std::unique_ptr<GoldDrop>> goldDrops_;
    
    void removeInactiveGold();
};