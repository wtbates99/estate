#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include "player.h"
#include "enemies/enemy_factory.h"
#include "config.h"
#include "minimap.h"
#include "background.h"
#include "background_menu.h"
#include "backgrounds/background_factory.h"

int main() {
    // Create a window using config values
    sf::RenderWindow window(sf::VideoMode(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT), Config::WINDOW_TITLE);
    window.setFramerateLimit(60);

    // Create views
    sf::View view(sf::FloatRect(0, 0, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT));
    sf::View uiView(sf::FloatRect(0, 0, Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT));

    // Create and initialize background menu
    BackgroundMenu backgroundMenu;
    if (!backgroundMenu.initialize()) {
        std::cerr << "Failed to initialize background menu" << std::endl;
        return -1;
    }
    backgroundMenu.activate();

    // Game state variables
    enum GameState {
        BACKGROUND_SELECTION,
        PLAYING
    };
    GameState currentState = BACKGROUND_SELECTION;

    // Clock for delta time and FPS calculation
    sf::Clock clock;
    sf::Clock fpsClock;
    int frameCount = 0;
    float fps = 0;

    // Game objects (will be initialized after background selection)
    std::unique_ptr<Background> gameBackground;
    std::unique_ptr<Player> main_player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    
    // Debug text setup
    sf::Font debugFont;
    if (!debugFont.loadFromFile("assets/ARIAL.TTF")) {
        std::cerr << "Failed to load font from assets/ARIAL.TTF" << std::endl;
        return -1;
    }
    sf::Text debugText;
    debugText.setFont(debugFont);
    debugText.setCharacterSize(14);
    debugText.setFillColor(sf::Color::White);

    // Main game loop
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        
        // Calculate FPS
        frameCount++;
        if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
            fps = frameCount;
            frameCount = 0;
            fpsClock.restart();
        }

        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            if (currentState == BACKGROUND_SELECTION) {
                backgroundMenu.handleEvent(event);
                
                // Check if background selection is complete
                if (!backgroundMenu.isActive()) {
                    // Create background based on selection
                    BackgroundType selectedType = backgroundMenu.getSelectedBackground();
                    gameBackground = createBackgroundWithFallback(selectedType, BackgroundType::FOREST);
                    
                    if (!gameBackground) {
                        std::cout << "Warning: No background could be loaded. Using black background." << std::endl;
                        gameBackground = std::make_unique<Background>(); // Empty background
                    }
                    
                    // Initialize game objects
                    main_player = std::make_unique<Player>();
                    
                    // Create enemies
                    for (int i = 0; i < 15; i++) {
                        enemies.push_back(createRandomEnemy());
                    }
                    
                    // Set initial view center to player position
                    view.setCenter(main_player->getWorldPosition());
                    window.setView(view);
                    
                    currentState = PLAYING;
                }
            } else if (currentState == PLAYING) {
                // Handle game events
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                    // Add key to restart background selection
                    if (event.key.code == sf::Keyboard::B) {
                        backgroundMenu.activate();
                        currentState = BACKGROUND_SELECTION;
                    }
                }
            }
        }

        // Update based on current state
        if (currentState == BACKGROUND_SELECTION) {
            backgroundMenu.update(deltaTime);
        } else if (currentState == PLAYING) {
            // Update game state
            main_player->move(deltaTime);
            main_player->update(deltaTime, enemies);
            main_player->wrapPosition();
            
            // Update all enemies
            for (auto& enemy : enemies) {
                if (enemy->isAlive()) {
                    enemy->updateAI(main_player->getWorldPosition(), deltaTime);
                    enemy->move(deltaTime);
                    enemy->updatePosition(main_player->getWorldPosition(), sf::Vector2f(0, 0));
                }
            }

            // Update view to follow player
            view.setCenter(main_player->getWorldPosition());
            window.setView(view);

            // Check for collision between player and enemies
            for (auto& enemy : enemies) {
                if (enemy->isAlive() && main_player->getBounds().intersects(enemy->getBounds())) {
                    enemy->attack(*main_player);
                }
            }
        }

        // Render based on current state
        window.clear(sf::Color::Black);
        
        if (currentState == BACKGROUND_SELECTION) {
            window.setView(uiView);
            backgroundMenu.draw(window);
        } else if (currentState == PLAYING) {
            // Set game view for drawing game objects
            window.setView(view);

            // Draw background first (behind everything else)
            if (gameBackground) {
                gameBackground->draw(window, main_player->getWorldPosition());
            }

            // Draw game objects
            main_player->draw(window);
            for (const auto& enemy : enemies) {
                if (enemy->isAlive()) {
                    enemy->draw(window);
                }
            }

            // Draw debug information in game view
            if (Config::DEBUG_MODE) {
                if (Config::SHOW_COLLISION_BOXES) {
                    // Draw collision boxes
                    sf::FloatRect playerBounds = main_player->getBounds();
                    sf::RectangleShape playerBox(sf::Vector2f(playerBounds.width, playerBounds.height));
                    playerBox.setPosition(playerBounds.left, playerBounds.top);
                    playerBox.setFillColor(sf::Color::Transparent);
                    playerBox.setOutlineColor(sf::Color::Green);
                    playerBox.setOutlineThickness(1);
                    window.draw(playerBox);

                    for (const auto& enemy : enemies) {
                        if (enemy->isAlive()) {
                            sf::FloatRect enemyBounds = enemy->getBounds();
                            sf::RectangleShape enemyBox(sf::Vector2f(enemyBounds.width, enemyBounds.height));
                            enemyBox.setPosition(enemyBounds.left, enemyBounds.top);
                            enemyBox.setFillColor(sf::Color::Transparent);
                            enemyBox.setOutlineColor(sf::Color::Red);
                            enemyBox.setOutlineThickness(1);
                            window.draw(enemyBox);
                        }
                    }
                }
            }

            // Switch to UI view for minimap and FPS
            window.setView(uiView);
            Minimap::draw(window, *main_player, enemies);

            if (Config::DEBUG_MODE && Config::SHOW_FPS) {
                // Count alive enemies for debug display
                int aliveCount = 0;
                for (const auto& enemy : enemies) {
                    if (enemy->isAlive()) aliveCount++;
                }
                
                std::string debugInfo = "FPS: " + std::to_string(static_cast<int>(fps)) + "\n";
                debugInfo += "Alive Enemies: " + std::to_string(aliveCount) + "\n";
                debugInfo += "Press 'B' to change background";
                
                debugText.setString(debugInfo);
                debugText.setPosition(10, 10);
                window.draw(debugText);
            }
        }

        window.display();
    }

    return 0;
}
