#include <SFML/Graphics.hpp>
#include <vector>
#include "clases.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tank Wars");

    Tank player1(100, 100, 100, 5);
    Wall wall1(300, 200, 100, 50, true, 50);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Movement control for player 1
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            player1.move(0, -1);  // Move up
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            player1.move(0, 1);  // Move down
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            player1.move(-1, 0);  // Move left
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            player1.move(1, 0);  // Move right
        }

        // Shooting control for player 1
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            player1.shoot(5, 0);  // Shoot right
        }

        // Update bullets
        for (Bullet& b : player1.bullets) {
            b.move();
        }

        // Clear and draw everything
        window.clear();
        player1.draw(window);
        wall1.draw(window);
        window.display();
    }

    return 0;
}
