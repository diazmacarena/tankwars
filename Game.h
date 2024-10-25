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
    const float shootInterval = 0.25f; // Intervalo mínimo entre disparos
    const float reloadTime = 3.0f;     // Tiempo de recarga de balas (en segundos)
    const int maxBullets = 3;          // Máximo de balas que puede tener un jugador
    int player1Bullets = 2;            // Balas actuales del jugador 1
    int player2Bullets = 2;            // Balas actuales del jugador 2

    sf::Clock shootClockPlayer1;       // Reloj para el intervalo de disparos del jugador 1
    sf::Clock shootClockPlayer2;       // Reloj para el intervalo de disparos del jugador 2
    sf::Clock reloadClockPlayer1;      // Reloj para recargar balas del jugador 1
    sf::Clock reloadClockPlayer2;      // Reloj para recargar balas del jugador 2

    // Muros del borde de la ventana
    sf::RectangleShape muroIzquierdo;
    sf::RectangleShape muroDerecho;
    sf::RectangleShape muroSuperior;
    sf::RectangleShape muroInferior;

public:
    // Constructor
    Game() : window(sf::VideoMode(1920, 1080), "Tanks Multiplayer"), 
             player1("tank1.png", 700, 300), 
             player2("tank2.png", 600, 200) {
        // Configurar los muros
        muroIzquierdo.setSize(sf::Vector2f(40, 1080)); // Muro a la izquierda
        muroIzquierdo.setPosition(0, 0);
        muroIzquierdo.setFillColor(sf::Color::White);

        muroDerecho.setSize(sf::Vector2f(40, 1080));   // Muro a la derecha
        muroDerecho.setPosition(1910, 0);
        muroDerecho.setFillColor(sf::Color::White);

        muroSuperior.setSize(sf::Vector2f(1920, 40));  // Muro superior
        muroSuperior.setPosition(0, 0);
        muroSuperior.setFillColor(sf::Color::Green);

        muroInferior.setSize(sf::Vector2f(1920, 40));  // Muro inferior
        muroInferior.setPosition(0, 1070);
        muroInferior.setFillColor(sf::Color::Green);
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
            moverTanque(player1, movementSpeed);  // Mueve el tanque hacia adelante
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            moverTanque(player1, -movementSpeed);  // Mueve el tanque hacia atrás
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            player1.rotate(-rotationSpeed);  // Rotación hacia la izquierda
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            player1.rotate(rotationSpeed);   // Rotación hacia la derecha
        }

        // Solo disparar si ha pasado suficiente tiempo desde el último disparo
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootClockPlayer1.getElapsedTime().asSeconds() >= shootInterval) {
            shootBullet(player1, player1Bullets, shootClockPlayer1, reloadClockPlayer1);
        }

        // Controles de jugador 2
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            moverTanque(player2, movementSpeed);  // Mueve el tanque hacia adelante
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            moverTanque(player2, -movementSpeed);  // Mueve el tanque hacia atrás
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            player2.rotate(-rotationSpeed);  // Rotación hacia la izquierda
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            player2.rotate(rotationSpeed);   // Rotación hacia la derecha
        }

        // Solo disparar si ha pasado suficiente tiempo desde el último disparo
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && shootClockPlayer2.getElapsedTime().asSeconds() >= shootInterval) {
            shootBullet(player2, player2Bullets, shootClockPlayer2, reloadClockPlayer2);
        }
    }

    // Mover el tanque y verificar colisiones con los muros
    void moverTanque(Tank &player, float speed) {
        float angleRadians = degreesToRadians(player.sprite.getRotation());
        float dx = std::cos(angleRadians) * speed;
        float dy = std::sin(angleRadians) * speed;

        // Movimiento provisional
        player.move(dx, dy);

        // Verificar colisiones con los muros
        if (player.sprite.getGlobalBounds().intersects(muroIzquierdo.getGlobalBounds()) ||
            player.sprite.getGlobalBounds().intersects(muroDerecho.getGlobalBounds()) ||
            player.sprite.getGlobalBounds().intersects(muroSuperior.getGlobalBounds()) ||
            player.sprite.getGlobalBounds().intersects(muroInferior.getGlobalBounds())) {
            // Si colisiona, revertir el movimiento
            player.move(-dx, -dy);
        }
    }

    // Actualizar el estado del juego (balas, colisiones)
    void update() {
        for (auto &bullet : bullets) {
            bullet.update();

            // Verificar colisión de la bala con los muros
            if (bullet.sprite.getGlobalBounds().intersects(muroIzquierdo.getGlobalBounds()) ||
                bullet.sprite.getGlobalBounds().intersects(muroDerecho.getGlobalBounds())) {
                bullet.velocity.x = -bullet.velocity.x;  // Invertir dirección en el eje X
                bullet.collisionCount++;  // Incrementar el contador de colisiones

            }

            if (bullet.sprite.getGlobalBounds().intersects(muroSuperior.getGlobalBounds()) ||
                bullet.sprite.getGlobalBounds().intersects(muroInferior.getGlobalBounds())) {
                bullet.velocity.y = -bullet.velocity.y;  // Invertir dirección en el eje Y
                bullet.collisionCount++;  // Incrementar el contador de colisiones

            }

            // Desactivar la bala después de 5 colisiones
            if (bullet.collisionCount >= 5) {
            bullet.isActive = false;
            }


        // Verificar colisión de la bala con los tanques
        if (bullet.checkCollision(player1.sprite)) {  // Usar player1.sprite
            player1.restarVida();  // Restar una vida al tanque
            std::cout << "Jugador 1 ha recibido un impacto, vidas restantes: " << player1.vidas << std::endl;
            bullet.isActive = false;  // Desactivar la bala después de impactar
        }
        if (bullet.checkCollision(player2.sprite)) {  // Usar player2.sprite
            player2.restarVida();  // Restar una vida al tanque
            std::cout << "Jugador 2 ha recibido un impacto, vidas restantes: " << player2.vidas << std::endl;
            bullet.isActive = false;  // Desactivar la bala después de impactar
        }


        // Verificar si algún tanque ha sido destruido
        if (player1.estaDestruido()) {
            std::cout << "Jugador 1 ha sido destruido." << std::endl;
        }
        if (player2.estaDestruido()) {
            std::cout << "Jugador 2 ha sido destruido." << std::endl;
        }
    }


        // Recarga automática de balas cada 3 segundos
        if (reloadClockPlayer1.getElapsedTime().asSeconds() >= reloadTime && player1Bullets < maxBullets) {
            player1Bullets++;
            reloadClockPlayer1.restart();  // Reiniciar el reloj de recarga
        }
        if (reloadClockPlayer2.getElapsedTime().asSeconds() >= reloadTime && player2Bullets < maxBullets) {
            player2Bullets++;
            reloadClockPlayer2.restart();  // Reiniciar el reloj de recarga
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

        // Dibujar los muros
        window.draw(muroIzquierdo);
        window.draw(muroDerecho);
        window.draw(muroSuperior);
        window.draw(muroInferior);

        window.display();
    }

    // Disparar una bala desde el tanque
    void shootBullet(Tank &player, int &bulletCount, sf::Clock &shootClock, sf::Clock &reloadClock) {
        if (bulletCount > 0) {
            float angleRadians = degreesToRadians(player.sprite.getRotation());
            sf::Vector2f direction(std::cos(angleRadians), std::sin(angleRadians));

            // Crear una nueva bala, pasando ahora &player como puntero al tanque
            bullets.emplace_back("bullet.png", player.sprite.getPosition().x, player.sprite.getPosition().y, direction, &player);

            bulletCount--;  // Decrementar el contador de balas
            shootClock.restart();
            reloadClock.restart();
        }
    }


};
