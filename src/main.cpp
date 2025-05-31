#include <SFML/Graphics.hpp>
#include "player.h"
#include "enemy.h"
#include "config.h"

int main() {
    // Create a window using config values
    sf::RenderWindow window(sf::VideoMode(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT), Config::WINDOW_TITLE);
    window.setFramerateLimit(60);

    // Clock for delta time and FPS calculation
    sf::Clock clock;
    sf::Clock fpsClock;
    int frameCount = 0;
    float fps = 0;

    // Create a player and enemy
    Player main_player;
    Enemy enemy;

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
        enemy.move(deltaTime);

        // Check for collision between player and enemy
        if (main_player.getBounds().intersects(enemy.getBounds())) {
            enemy.attack(main_player);
        }

        // Clear the window
        window.clear(sf::Color::Black);

        // Draw game objects
        main_player.draw(window);
        enemy.draw(window);

        // Draw debug information
        if (Config::DEBUG_MODE) {
            if (Config::SHOW_FPS) {
                debugText.setString("FPS: " + std::to_string(static_cast<int>(fps)));
                debugText.setPosition(10, 10);
                window.draw(debugText);
            }

            if (Config::SHOW_COLLISION_BOXES) {
                // Draw collision boxes
                sf::RectangleShape playerBox(main_player.getBounds().getSize());
                playerBox.setPosition(main_player.getBounds().getPosition());
                playerBox.setFillColor(sf::Color::Transparent);
                playerBox.setOutlineColor(sf::Color::Green);
                playerBox.setOutlineThickness(1);
                window.draw(playerBox);

                sf::RectangleShape enemyBox(enemy.getBounds().getSize());
                enemyBox.setPosition(enemy.getBounds().getPosition());
                enemyBox.setFillColor(sf::Color::Transparent);
                enemyBox.setOutlineColor(sf::Color::Red);
                enemyBox.setOutlineThickness(1);
                window.draw(enemyBox);
            }
        }

        window.display();
    }

    return 0;
}
