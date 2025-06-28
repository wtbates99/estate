#include "gold_manager.h"
#include "player.h"
#include <algorithm>

GoldManager::GoldManager() {
}

void GoldManager::update(float deltaTime, Player& player) {
    // Update all gold drops
    for (auto& gold : goldDrops_) {
        gold->update(deltaTime);
        
        // Check for collection
        if (gold->checkCollision(player)) {
            player.addGold(gold->getValue());
        }
    }
    
    // Remove inactive gold drops
    removeInactiveGold();
}

void GoldManager::draw(sf::RenderWindow& window) const {
    for (const auto& gold : goldDrops_) {
        gold->draw(window);
    }
}

void GoldManager::spawnGold(const sf::Vector2f& position, int value) {
    goldDrops_.push_back(std::make_unique<GoldDrop>(position, value));
}

void GoldManager::clear() {
    goldDrops_.clear();
}

void GoldManager::removeInactiveGold() {
    goldDrops_.erase(
        std::remove_if(goldDrops_.begin(), goldDrops_.end(),
            [](const std::unique_ptr<GoldDrop>& gold) {
                return !gold->isActive();
            }),
        goldDrops_.end()
    );
}