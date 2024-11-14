#include "Game.h"
#include <cmath>
#include <iostream>

Game::Game() : window(sf::VideoMode(1920, 1080), "Tanks Multiplayer"),
               player1("tank1.png", 700, 300),
               player2("tank2.png", 600, 200) {
    muroIzquierdo.setSize(sf::Vector2f(40, 1080));
    muroIzquierdo.setPosition(0, 0);
    muroIzquierdo.setFillColor(sf::Color::White);

    muroDerecho.setSize(sf::Vector2f(40, 1080));
    muroDerecho.setPosition(1910, 0);
    muroDerecho.setFillColor(sf::Color::White);

    muroSuperior.setSize(sf::Vector2f(1920, 40));
    muroSuperior.setPosition(0, 0);
    muroSuperior.setFillColor(sf::Color::Green);

    muroInferior.setSize(sf::Vector2f(1920, 40));
    muroInferior.setPosition(0, 1070);
    muroInferior.setFillColor(sf::Color::Green);
}

float Game::degreesToRadians(float degrees) {
    return degrees * (3.1416f / 180.0f);
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        moverTanque(player1, movementSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        moverTanque(player1, -movementSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        player1.rotate(-rotationSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        player1.rotate(rotationSpeed);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootClockPlayer1.getElapsedTime().asSeconds() >= shootInterval) {
        shootBullet(player1, player1Bullets, shootClockPlayer1, reloadClockPlayer1);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        moverTanque(player2, movementSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        moverTanque(player2, -movementSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        player2.rotate(-rotationSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        player2.rotate(rotationSpeed);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && shootClockPlayer2.getElapsedTime().asSeconds() >= shootInterval) {
        shootBullet(player2, player2Bullets, shootClockPlayer2, reloadClockPlayer2);
    }
}

void Game::moverTanque(Tank &player, float speed) {
    float angleRadians = degreesToRadians(player.sprite.getRotation());
    float dx = std::cos(angleRadians) * speed;
    float dy = std::sin(angleRadians) * speed;

    player.move(dx, dy);

    if (player.sprite.getGlobalBounds().intersects(muroIzquierdo.getGlobalBounds()) ||
        player.sprite.getGlobalBounds().intersects(muroDerecho.getGlobalBounds()) ||
        player.sprite.getGlobalBounds().intersects(muroSuperior.getGlobalBounds()) ||
        player.sprite.getGlobalBounds().intersects(muroInferior.getGlobalBounds())) {
        player.move(-dx, -dy);
    }
}


void Game::update() {
    for (auto &bullet : bullets) {
        bullet.update();

        if (bullet.sprite.getGlobalBounds().intersects(muroIzquierdo.getGlobalBounds()) ||
            bullet.sprite.getGlobalBounds().intersects(muroDerecho.getGlobalBounds())) {
            bullet.velocity.x = -bullet.velocity.x;
            bullet.collisionCount++;
        }

        if (bullet.sprite.getGlobalBounds().intersects(muroSuperior.getGlobalBounds()) ||
            bullet.sprite.getGlobalBounds().intersects(muroInferior.getGlobalBounds())) {
            bullet.velocity.y = -bullet.velocity.y;
            bullet.collisionCount++;
        }

        if (bullet.collisionCount >= 5) {
            bullet.isActive = false;
        }

        // Verificar colisiones con los tanques y restar vidas
        if (bullet.checkCollision(player1.sprite)) {
            player1.restarVida();
            std::cout << "Jugador 1 ha recibido un impacto, vidas restantes: " << player1.vidas << std::endl;
            bullet.isActive = false;
        }
        if (bullet.checkCollision(player2.sprite)) {
            player2.restarVida();
            std::cout << "Jugador 2 ha recibido un impacto, vidas restantes: " << player2.vidas << std::endl;
            bullet.isActive = false;
        }

        // Mostrar mensaje si algún jugador es destruido
        if (player1.estaDestruido()) {
            std::cout << "Jugador 1 ha sido destruido." << std::endl;
        }
        if (player2.estaDestruido()) {
            std::cout << "Jugador 2 ha sido destruido." << std::endl;
        }
    }

    // Recargar balas de cada jugador si es necesario
    if (reloadClockPlayer1.getElapsedTime().asSeconds() >= reloadTime && player1Bullets < maxBullets) {
        player1Bullets++;
        reloadClockPlayer1.restart();
    }
    if (reloadClockPlayer2.getElapsedTime().asSeconds() >= reloadTime && player2Bullets < maxBullets) {
        player2Bullets++;
        reloadClockPlayer2.restart();
    }
}


void Game::render() {
    window.clear();
    player1.draw(window);
    player2.draw(window);

    for (auto &bullet : bullets) {
        bullet.draw(window);
    }

    window.draw(muroIzquierdo);
    window.draw(muroDerecho);
    window.draw(muroSuperior);
    window.draw(muroInferior);

    window.display();
}

void Game::shootBullet(Tank &player, int &bulletCount, sf::Clock &shootClock, sf::Clock &reloadClock) {
    if (bulletCount > 0) {
        float angleRadians = degreesToRadians(player.sprite.getRotation());
        sf::Vector2f direction(std::cos(angleRadians), std::sin(angleRadians));

        bullets.emplace_back("bullet.png", player.sprite.getPosition().x, player.sprite.getPosition().y, direction, &player);

        bulletCount--;
        shootClock.restart();
        reloadClock.restart();
    }
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}
