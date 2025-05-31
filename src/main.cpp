#include <SFML/Graphics.hpp>
#include <vector>
#include "player.h"
#include "enemy.h"
#include "config.h"
#include "minimap.h"

int main() {
    // Create a window using config values
    sf::RenderWindow window(sf::VideoMode(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT), Config::WINDOW_TITLE);
    window.setFramerateLimit(60);

    // Create a view that will follow the player
    sf::View view(sf::FloatRect(0, 0, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT));
    window.setView(view);

    // Create a fixed view for UI elements
    sf::View uiView(sf::FloatRect(0, 0, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT));

    // Clock for delta time and FPS calculation
    sf::Clock clock;
    sf::Clock fpsClock;
    int frameCount = 0;
    float fps = 0;

    // Create a player and enemies
    Player main_player;
    std::vector<Enemy> enemies;
    
    // Create a mix of different enemy types
    for (int i = 0; i < 15; i++) {
        EnemyType type;
        int typeRoll = rand() % 100;
        
        if (typeRoll < 40) {
            type = EnemyType::GRUNT;     // 40% chance
        } else if (typeRoll < 60) {
            type = EnemyType::SCOUT;     // 20% chance
        } else if (typeRoll < 80) {
            type = EnemyType::BRUTE;     // 20% chance
        } else {
            type = EnemyType::ASSASSIN;  // 20% chance
        }
        
        enemies.push_back(Enemy(type));
    }

    // Set initial view center to player position
    view.setCenter(main_player.getWorldPosition());
    window.setView(view);

    // Debug text setup
    sf::Font debugFont;
    if (!debugFont.loadFromFile("ARIAL.TTF")) {
        return -1;
    }
    sf::Text debugText;
    debugText.setFont(debugFont);
    debugText.setCharacterSize(14);
    debugText.setFillColor(sf::Color::White);

    // Main game loop
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Add debug controls
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }

        float deltaTime = clock.restart().asSeconds();

        // Calculate FPS
        frameCount++;
        if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
            fps = frameCount;
            frameCount = 0;
            fpsClock.restart();
        }

        // Update game state
        main_player.move(deltaTime);
        main_player.wrapPosition();  // Handle world wrapping
        
        // Update all enemies
        for (auto& enemy : enemies) {
            enemy.move(deltaTime);
            enemy.updatePosition(main_player.getWorldPosition(), sf::Vector2f(0, 0));
        }

        // Update view to follow player
        view.setCenter(main_player.getWorldPosition());
        window.setView(view);

        // Check for collision between player and enemies
        for (auto& enemy : enemies) {
            if (main_player.getBounds().intersects(enemy.getBounds())) {
                enemy.attack(main_player);
            }
        }

        // Clear the window
        window.clear(sf::Color::Black);

        // Set game view for drawing game objects
        window.setView(view);

        // Draw game objects
        main_player.draw(window);
        for (const auto& enemy : enemies) {
            enemy.draw(window);
        }

        // Draw debug information in game view
        if (Config::DEBUG_MODE) {
            if (Config::SHOW_COLLISION_BOXES) {
                // Draw collision boxes
                sf::RectangleShape playerBox(main_player.getBounds().getSize());
                playerBox.setPosition(main_player.getBounds().getPosition());
                playerBox.setFillColor(sf::Color::Transparent);
                playerBox.setOutlineColor(sf::Color::Green);
                playerBox.setOutlineThickness(1);
                window.draw(playerBox);

                for (const auto& enemy : enemies) {
                    sf::RectangleShape enemyBox(enemy.getBounds().getSize());
                    enemyBox.setPosition(enemy.getBounds().getPosition());
                    enemyBox.setFillColor(sf::Color::Transparent);
                    enemyBox.setOutlineColor(sf::Color::Red);
                    enemyBox.setOutlineThickness(1);
                    window.draw(enemyBox);
                }
            }
        }

        // Switch to UI view for minimap and FPS
        window.setView(uiView);
        Minimap::draw(window, main_player, enemies);

        if (Config::DEBUG_MODE && Config::SHOW_FPS) {
            // Count enemy types for debug display
            int gruntCount = 0, scoutCount = 0, bruteCount = 0, assassinCount = 0;
            for (const auto& enemy : enemies) {
                if (!enemy.isAlive()) continue;
                switch (enemy.getType()) {
                    case EnemyType::GRUNT: gruntCount++; break;
                    case EnemyType::SCOUT: scoutCount++; break;
                    case EnemyType::BRUTE: bruteCount++; break;
                    case EnemyType::ASSASSIN: assassinCount++; break;
                }
            }
            
            std::string debugInfo = "FPS: " + std::to_string(static_cast<int>(fps)) + "\n";
            debugInfo += "Enemies - Grunt:" + std::to_string(gruntCount) + 
                        " Scout:" + std::to_string(scoutCount) + 
                        " Brute:" + std::to_string(bruteCount) + 
                        " Assassin:" + std::to_string(assassinCount);
            
            debugText.setString(debugInfo);
            debugText.setPosition(10, 10);
            window.draw(debugText);
        }

        window.display();
    }

    return 0;
}
