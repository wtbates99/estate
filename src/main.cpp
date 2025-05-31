#include <SFML/Graphics.hpp>

int main() {
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML 2D Game");

    // Create a rectangle
    sf::RectangleShape rectangle(sf::Vector2f(50.f, 50.f));
    rectangle.setFillColor(sf::Color::Green);
    rectangle.setPosition(400.f, 300.f); // Center of window

    // Clock for delta time
    sf::Clock clock;

    // Main game loop
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Get delta time
        float deltaTime = clock.restart().asSeconds();

        // Handle input
        float speed = 200.f; // Pixels per second
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            rectangle.move(-speed * deltaTime, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            rectangle.move(speed * deltaTime, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            rectangle.move(0.f, -speed * deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            rectangle.move(0.f, speed * deltaTime);
        }

        // Clear, draw, and display
        window.clear(sf::Color::Black);
        window.draw(rectangle);
        window.display();
    }

    return 0;
}
