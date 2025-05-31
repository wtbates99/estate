#include <SFML/Graphics.hpp>
#include "player.h"
#include "enemy.h"

int main() {
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "estate");


    // Clock for delta time
    sf::Clock clock;

    // Create a player
    Player main_player;

    // Create an enemy
    Enemy enemy;

    // Main game loop
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        float deltaTime = clock.restart().asSeconds();

        main_player.move(deltaTime);

        window.clear(sf::Color::Black);
        main_player.draw(window);
        enemy.draw(window);
        window.display();
    }

    return 0;
}
