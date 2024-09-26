#include <SFML/Graphics.hpp>
#include "Tank.h"
#include "Bullet.h"
#include <vector>

class Game {
private:
    sf::RenderWindow window;
    Tank player1;
    Tank player2;
    std::vector<Bullet> bullets;
    sf::Texture bulletTexture;

public:
    // Constructor
    Game() : window(sf::VideoMode(1500, 1100), "Tanks Multiplayer"), player1("tank1.png", 1, 1), player2("tank2.png", 1200, 800) {
        if (!bulletTexture.loadFromFile("Bullet.png")) {
            // Handle error
        }
    }

    // Bucle principal del juego
    void run() {
        while (window.isOpen()) {
            processEvents();
            update();
            render();
        }
    }

private:
    // Procesar los eventos (teclas, cierre de ventana, etc.)
    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        
        // Controles de jugador 1
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            player1.move(0, -1);  // Arriba
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            player1.move(0, 1);   // Abajo
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            player1.move(-1,0);
            player1.inRotate();  // Izquierda
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            player1.move(1, 0);  
            player1.rotate(); // Derecha
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            shootBullet(player1);
        }

        // Controles de jugador 2
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            player2.move(0, -1);  // Arriba
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            player2.move(0, 1);   // Abajo
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            player2.move(-1, 0);
            player2.inRotate();  // Izquierda
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            player2.move(1, 0);   // Derecha
            player2.rotate();   
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            shootBullet(player2);
        }
    }

    // Actualizar el estado del juego (balas, colisiones)
    void update() {
        for (auto &bullet : bullets) {
            bullet.update();
            if (bullet.checkCollision(player1.sprite)) {
                // Colisión con jugador 1
            }
            if (bullet.checkCollision(player2.sprite)) {
                // Colisión con jugador 2
            }
        }
    }

    // Dibujar los objetos en pantalla
    void render() {
        window.clear();
        player1.draw(window);
        player2.draw(window);

        for (auto &bullet : bullets) {
            bullet.draw(window);
        }

        window.display();
    }

    // Disparar una bala desde el tanque
    void shootBullet(Tank &player) {
        sf::Vector2f direction = player.direction;
        if (direction.x == 0 && direction.y == 0) {
            direction.x = 1;  // Disparo por defecto
        }
        bullets.emplace_back("bullet.png", player.sprite.getPosition().x, player.sprite.getPosition().y, direction);
        
    }
};
