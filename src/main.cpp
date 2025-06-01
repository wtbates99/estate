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
#include "health_pack.h"

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
        PLAYING,
        TALENT_SELECTION
    };
    GameState currentState = BACKGROUND_SELECTION;

    // Clock for delta time and FPS calculation
    sf::Clock clock;
    sf::Clock fpsClock;
    int frameCount = 0;
    float fps = 0;
    
    // Enemy spawning system
    sf::Clock enemySpawnClock;
    const float ENEMY_SPAWN_INTERVAL = 3.0f; // Spawn new enemy every 3 seconds
    const int MIN_ENEMIES = 10; // Minimum number of enemies to maintain

    // Game objects (will be initialized after background selection)
    std::unique_ptr<Background> gameBackground;
    std::unique_ptr<Player> main_player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<HealthPack>> healthPacks; // Health pack container
    
    // Talent selection state
    int selectedTalentIndex = 0;
    sf::Vector2f talentTreeOffset(50.f, 50.f);  // Adjusted for new positioning
    
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
            } else if (currentState == TALENT_SELECTION) {
                // Handle talent selection events
                if (event.type == sf::Event::KeyPressed) {
                    auto availableTalents = main_player->getTalentTree().getAvailableTalents();
                    
                    if (event.key.code == sf::Keyboard::Left && selectedTalentIndex > 0) {
                        selectedTalentIndex--;
                    } else if (event.key.code == sf::Keyboard::Right && selectedTalentIndex < static_cast<int>(availableTalents.size()) - 1) {
                        selectedTalentIndex++;
                    } else if (event.key.code == sf::Keyboard::Enter && !availableTalents.empty()) {
                        // Select the talent
                        if (selectedTalentIndex < static_cast<int>(availableTalents.size())) {
                            main_player->selectTalent(availableTalents[selectedTalentIndex]);
                            selectedTalentIndex = 0; // Reset selection
                            currentState = PLAYING; // Return to game
                        }
                    } else if (event.key.code == sf::Keyboard::Escape) {
                        // Skip talent selection (for debugging)
                        currentState = PLAYING;
                    }
                }
            }
        }

        // Update based on current state
        if (currentState == BACKGROUND_SELECTION) {
            backgroundMenu.update(deltaTime);
        } else if (currentState == PLAYING) {
            // Check if player needs to level up
            if (main_player->needsLevelUp()) {
                main_player->processLevelUp();
                currentState = TALENT_SELECTION;
                selectedTalentIndex = 0;
            } else {
                // Update game state normally
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

                // Check for dead enemies and award experience
                auto it = enemies.begin();
                while (it != enemies.end()) {
                    if (!(*it)->isAlive()) {
                        // Check if this enemy should drop a health pack
                        if (Enemy::shouldDropHealthPack(*main_player)) {
                            sf::Vector2f healthPackPos = (*it)->getWorldPosition();
                            healthPacks.push_back(std::make_unique<HealthPack>(healthPackPos));
                            std::cout << "Health pack dropped at: (" << healthPackPos.x << ", " << healthPackPos.y << ")" << std::endl;
                            std::cout << "Player health: " << main_player->getHealth() << "/" << main_player->getMaxHealth() << std::endl;
                        }
                        
                        // Award experience to player before removing the enemy
                        main_player->gainExperience((*it)->getExperienceValue());
                        it = enemies.erase(it);
                    } else {
                        ++it;
                    }
                }
                
                // Update health packs
                auto healthPackIt = healthPacks.begin();
                while (healthPackIt != healthPacks.end()) {
                    (*healthPackIt)->update(deltaTime);
                    
                    // Check for collision with player
                    if ((*healthPackIt)->checkCollision(*main_player)) {
                        main_player->healPlayer((*healthPackIt)->getHealAmount());
                        std::cout << "Health pack collected! Player healed by " << (*healthPackIt)->getHealAmount() << " HP" << std::endl;
                        healthPackIt = healthPacks.erase(healthPackIt);
                    } else if ((*healthPackIt)->isExpired()) {
                        // Remove expired health packs
                        std::cout << "Health pack expired and removed" << std::endl;
                        healthPackIt = healthPacks.erase(healthPackIt);
                    } else {
                        ++healthPackIt;
                    }
                }
                
                // Spawn new enemies if needed
                if (enemies.size() < MIN_ENEMIES || enemySpawnClock.getElapsedTime().asSeconds() > ENEMY_SPAWN_INTERVAL) {
                    enemies.push_back(createRandomEnemy());
                    enemySpawnClock.restart();
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
                
                // Give player experience for testing (remove this later)
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
                    main_player->gainExperience(10);
                }
                
                // TEMPORARY: Spawn health pack for testing (remove this later)
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
                    sf::Vector2f playerPos = main_player->getWorldPosition();
                    sf::Vector2f healthPackPos = playerPos + sf::Vector2f(50.f, 0.f); // Spawn to the right of player
                    healthPacks.push_back(std::make_unique<HealthPack>(healthPackPos));
                    std::cout << "Manual health pack spawned at: (" << healthPackPos.x << ", " << healthPackPos.y << ")" << std::endl;
                }
            }
        }
        // TALENT_SELECTION state doesn't need updates - it's paused

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
            
            // Draw health packs
            for (const auto& healthPack : healthPacks) {
                healthPack->draw(window);
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
                    
                    // Draw health pack collision boxes
                    for (const auto& healthPack : healthPacks) {
                        sf::Vector2f packPos = healthPack->getPosition();
                        sf::RectangleShape packBox(sf::Vector2f(20.f, 20.f));
                        packBox.setPosition(packPos.x - 10.f, packPos.y - 10.f);
                        packBox.setFillColor(sf::Color::Transparent);
                        packBox.setOutlineColor(sf::Color::Cyan);
                        packBox.setOutlineThickness(1);
                        window.draw(packBox);
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
                debugInfo += "Health Packs: " + std::to_string(healthPacks.size()) + "\n";
                debugInfo += "Press 'B' to change background\n";
                debugInfo += "Press 'X' to gain experience (debug)";
                
                debugText.setString(debugInfo);
                debugText.setPosition(10, 10);
                window.draw(debugText);
            }
        } else if (currentState == TALENT_SELECTION) {
            // Draw talent selection screen
            window.setView(uiView);
            
            // Draw semi-transparent overlay
            sf::RectangleShape overlay(sf::Vector2f(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT));
            overlay.setFillColor(sf::Color(0, 0, 0, 150));
            window.draw(overlay);
            
            // Draw title
            sf::Text titleText;
            titleText.setFont(debugFont);
            titleText.setString("LEVEL UP! Choose a Talent");
            titleText.setCharacterSize(32);
            titleText.setFillColor(sf::Color::Yellow);
            titleText.setPosition(Config::WINDOW_WIDTH / 2 - 200, 50);
            window.draw(titleText);
            
            // Draw talent tree
            main_player->getTalentTree().draw(window, talentTreeOffset);
            
            // Draw instructions
            sf::Text instructText;
            instructText.setFont(debugFont);
            instructText.setString("Use LEFT/RIGHT arrows to navigate, ENTER to select, ESC to skip");
            instructText.setCharacterSize(16);
            instructText.setFillColor(sf::Color::White);
            instructText.setPosition(50, Config::WINDOW_HEIGHT - 100);
            window.draw(instructText);
            
            // Highlight selected talent
            auto availableTalents = main_player->getTalentTree().getAvailableTalents();
            if (!availableTalents.empty() && selectedTalentIndex < static_cast<int>(availableTalents.size())) {
                const auto& nodes = main_player->getTalentTree().getNodes();
                if (availableTalents[selectedTalentIndex] < static_cast<int>(nodes.size())) {
                    sf::Vector2f talentPos = nodes[availableTalents[selectedTalentIndex]].position + talentTreeOffset;
                    sf::RectangleShape highlight(sf::Vector2f(180.f, 120.f));
                    highlight.setPosition(talentPos);
                    highlight.setFillColor(sf::Color::Transparent);
                    highlight.setOutlineColor(sf::Color::Yellow);
                    highlight.setOutlineThickness(3.f);
                    window.draw(highlight);
                }
            }
        }

        window.display();
    }

    return 0;
}
