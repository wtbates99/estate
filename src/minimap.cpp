#include "minimap.h"
#include "config.h"
#include "shop.h"

void Minimap::draw(sf::RenderWindow& window, const Player& player, const std::vector<std::unique_ptr<Enemy>>& enemies, const Shop* shop) {
    // Create minimap background
    sf::RectangleShape minimapBackground(sf::Vector2f(Config::MINIMAP_SIZE, Config::MINIMAP_SIZE));
    minimapBackground.setPosition(Config::WINDOW_WIDTH - Config::MINIMAP_SIZE - Config::MINIMAP_PADDING, 
                                Config::MINIMAP_PADDING);
    minimapBackground.setFillColor(sf::Color(0, 0, 0, 128));
    minimapBackground.setOutlineThickness(1);
    minimapBackground.setOutlineColor(sf::Color::White);
    
    // Draw minimap background
    window.draw(minimapBackground);
    
    // Calculate player position on minimap
    sf::CircleShape playerDot(Config::MINIMAP_PLAYER_SIZE);
    playerDot.setFillColor(sf::Color::Green);
    playerDot.setOrigin(Config::MINIMAP_PLAYER_SIZE, Config::MINIMAP_PLAYER_SIZE); // Center the origin
    sf::Vector2f playerPos = player.getWorldPosition();
    playerDot.setPosition(
        Config::WINDOW_WIDTH - Config::MINIMAP_SIZE - Config::MINIMAP_PADDING + 
        (playerPos.x * Config::MINIMAP_SCALE),
        Config::MINIMAP_PADDING + 
        (playerPos.y * Config::MINIMAP_SCALE)
    );
    
    // Draw player dot on minimap
    window.draw(playerDot);
    
    // Draw all alive enemy dots on minimap
    for (const auto& enemy : enemies) {
        if (enemy->isAlive()) {
            sf::CircleShape enemyDot(Config::MINIMAP_ENEMY_SIZE);
            enemyDot.setFillColor(sf::Color::Red);
            enemyDot.setOrigin(Config::MINIMAP_ENEMY_SIZE, Config::MINIMAP_ENEMY_SIZE); // Center the origin
            sf::Vector2f enemyPos = enemy->getWorldPosition();
            enemyDot.setPosition(
                Config::WINDOW_WIDTH - Config::MINIMAP_SIZE - Config::MINIMAP_PADDING + 
                (enemyPos.x * Config::MINIMAP_SCALE),
                Config::MINIMAP_PADDING + 
                (enemyPos.y * Config::MINIMAP_SCALE)
            );
            window.draw(enemyDot);
        }
    }
    
    // Draw shop on minimap if it exists
    if (shop && shop->isVisible()) {
        sf::CircleShape shopDot(6.0f); // Slightly larger than other dots
        shopDot.setFillColor(sf::Color::Yellow);
        shopDot.setOutlineColor(sf::Color(255, 215, 0)); // Gold outline
        shopDot.setOutlineThickness(2.0f);
        shopDot.setOrigin(6.0f, 6.0f);
        
        sf::Vector2f shopPos = shop->getPosition();
        shopDot.setPosition(
            Config::WINDOW_WIDTH - Config::MINIMAP_SIZE - Config::MINIMAP_PADDING + 
            (shopPos.x * Config::MINIMAP_SCALE),
            Config::MINIMAP_PADDING + 
            (shopPos.y * Config::MINIMAP_SCALE)
        );
        window.draw(shopDot);
    }
} 