#include <SFML/Graphics.hpp>
#include "Tank.h"
#include "Bullet.h"
#include <vector>
#include <cmath>

class Game {
private:
    sf::RenderWindow window;
    Tank player1;
    Tank player2;
    std::vector<Bullet> bullets;
    sf::Texture bulletTexture;
    const float movementSpeed = 1.0f;  // Velocidad de movimiento
    const float rotationSpeed = 0.3f;  // Velocidad de rotación

public:
    // Constructor
    Game() : window(sf::VideoMode(1500, 1100), "Tanks Multiplayer"), 
             player1("tank1.png", 700, 300), 
             player2("tank2.png", 600, 200) {}

    // Bucle principal del juego
    void run() {
        while (window.isOpen()) {
            processEvents();
            update();
            render();
        }
    }

private:
    // Función auxiliar para convertir grados a radianes
    float degreesToRadians(float degrees) {
        return degrees * (3.1416f / 180.0f);
    }

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
            float angleRadians = degreesToRadians(player1.sprite.getRotation());
            float dx = std::cos(angleRadians) * movementSpeed;
            float dy = std::sin(angleRadians) * movementSpeed;
            player1.move(dx, dy);  // Mueve el tanque hacia adelante
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            float angleRadians = degreesToRadians(player1.sprite.getRotation());
            float dx = std::cos(angleRadians) * -movementSpeed;
            float dy = std::sin(angleRadians) * -movementSpeed;
            player1.move(dx, dy);  // Mueve el tanque hacia atrás
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            player1.rotate(-rotationSpeed);  // Rotación hacia la izquierda
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            player1.rotate(rotationSpeed);   // Rotación hacia la derecha
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            shootBullet(player1);
        }
        
        // Controles de jugador 2
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            float angleRadians = degreesToRadians(player2.sprite.getRotation());
            float dx = std::cos(angleRadians) * movementSpeed;
            float dy = std::sin(angleRadians) * movementSpeed;
            player2.move(dx, dy);  // Mueve el tanque hacia adelante
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            float angleRadians = degreesToRadians(player2.sprite.getRotation());
            float dx = std::cos(angleRadians) * -movementSpeed;
            float dy = std::sin(angleRadians) * -movementSpeed;
            player2.move(dx, dy);  // Mueve el tanque hacia atrás
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            player2.rotate(-rotationSpeed);  // Rotación hacia la izquierda
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            player2.rotate(rotationSpeed);   // Rotación hacia la derecha
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
                std::cout << "bala de jugador 2 acerto" << std::endl;
            }
            if (bullet.checkCollision(player2.sprite)) {
                std::cout << "bala de jugador 1 acerto" << std::endl;
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
        // Calcular la dirección basada en la rotación actual del tanque
        float angleRadians = degreesToRadians(player.sprite.getRotation());
        sf::Vector2f direction(std::cos(angleRadians), std::sin(angleRadians));

        bullets.emplace_back("bullet.png", player.sprite.getPosition().x, player.sprite.getPosition().y, direction, player);
    }
};
