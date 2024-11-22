#include "Game.h"
#include <cmath>
#include <fstream>
#include <iostream>

// Tamaño de cada celda en el laberinto
const int TILE_SIZE = 40;

// Constructor
Game::Game() 
    : window(sf::VideoMode(1920, 1080), "Tanks Multiplayer"),
      player1("tank1.png", 200, 150),
      player2("tank2.png", 1600, 800) {
    cargarNivel(mapFile);  // Cargar el nivel desde un archivo
}

Game::Game(const std::string& nivel)
    : window(sf::VideoMode(1920, 1080), "Tanks Multiplayer"),
      player1("tank1.png", 200, 150), player2("tank2.png", 1600, 800) {
    cargarNivel(nivel);
}

float Game::degreesToRadians(float degrees) {
    return degrees * (3.1416f / 180.0f);
}

// Función para cargar el nivel desde un archivo
void Game::cargarNivel(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error al abrir el archivo de nivel: " << filename << std::endl;
        return;
    }

    int row = 0;
    std::string line;
    while (std::getline(file, line)) {
        for (size_t col = 0; col < line.size(); ++col) {
            if (line[col] == '2') {  // Muro destructible
                destructibleWalls.push_back(DestructibleWall(TILE_SIZE, TILE_SIZE, col * TILE_SIZE, row * TILE_SIZE, sf::Color(128, 0, 128))); // Morado
            }
            else if (line[col] == '1') {  // Muro normal
                walls.push_back(Wall(TILE_SIZE, TILE_SIZE, col * TILE_SIZE, row * TILE_SIZE, sf::Color::White)); // Blanco
            }
        }
        row++;
    }
}

// Función para procesar eventos de control
void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }

    // Control del jugador 1
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) 
        moverTanque(player1, movementSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) 
        moverTanque(player1, -movementSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) 
        player1.rotate(-rotationSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) 
        player1.rotate(rotationSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootClockPlayer1.getElapsedTime().asSeconds() >= shootInterval) {
        shootBullet(player1, player1Bullets, shootClockPlayer1, reloadClockPlayer1);
    }

    // Control del jugador 2
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) 
        moverTanque(player2, movementSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        moverTanque(player2, -movementSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        player2.rotate(-rotationSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
        player2.rotate(rotationSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && shootClockPlayer2.getElapsedTime().asSeconds() >= shootInterval) {
        shootBullet(player2, player2Bullets, shootClockPlayer2, reloadClockPlayer2);
    }
}

// Movimiento y manejo de colisiones del tanque
void Game::moverTanque(Tank &player, float speed) {
    float angleRadians = degreesToRadians(player.sprite.getRotation());
    float dx = std::cos(angleRadians) * speed;
    float dy = std::sin(angleRadians) * speed;

    // Guardar la última posición sin colisión
    static sf::Vector2f lastValidPosition = player.sprite.getPosition();
    player.move(dx, dy);

    // Función para calcular el porcentaje de solapamiento entre el tanque y el muro
    auto calculateOverlapPercentage = [&](const sf::FloatRect& tankBounds, const sf::FloatRect& wallBounds) -> float {
        float overlapX = std::max(0.0f, std::min(tankBounds.left + tankBounds.width, wallBounds.left + wallBounds.width) - 
                                      std::max(tankBounds.left, wallBounds.left));
        float overlapY = std::max(0.0f, std::min(tankBounds.top + tankBounds.height, wallBounds.top + wallBounds.height) - 
                                      std::max(tankBounds.top, wallBounds.top));

        float overlapArea = overlapX * overlapY;
        float tankArea = tankBounds.width * tankBounds.height;

        return (overlapArea / tankArea) * 100;  // Porcentaje de solapamiento
    };

    auto handleCollision = [&](const Wall& muro) {
        sf::FloatRect muroBounds = muro.getBounds();
        sf::FloatRect tankBounds = player.sprite.getGlobalBounds();
        
        float overlapPercentage = calculateOverlapPercentage(tankBounds, muroBounds);

        // Si el tanque tiene un solapamiento mayor o igual al 12%, restaurar a la última posición sin colisión
        if (overlapPercentage >= 3.0f) {
            player.sprite.setPosition(lastValidPosition);  // Restaurar a la posición sin colisión
        }
    };

    bool isColliding = false;

    // Verificar colisiones con muros destructibles
    for (auto& muro : destructibleWalls) {
        if (player.sprite.getGlobalBounds().intersects(muro.getBounds()) && !muro.isDestroyed()) {
            handleCollision(muro);
            isColliding = true;
            break;
        }
    }

    // Verificar colisiones con muros no destructibles
    for (auto& muro : walls) {
        if (player.sprite.getGlobalBounds().intersects(muro.getBounds())) {
            handleCollision(muro);
            isColliding = true;
            break;
        }
    }

    // Actualizar la última posición válida solo si no hay colisión
    if (!isColliding) {
        lastValidPosition = player.sprite.getPosition();
    }
}


// Actualización del estado del juego
void Game::update() {
    for (auto& bullet : bullets) {
        if (!bullet.isActive) continue;

        bullet.update();

        // Colisiones de la bala con muros destructibles
        for (auto& wall : destructibleWalls) {
            // Solo verificar colisión si el muro no está destruido
            if (!wall.isDestroyed() && bullet.sprite.getGlobalBounds().intersects(wall.getBounds())) {
                wall.hit();  // Destruir el muro destructible
                bullet.isActive = false;  // Desactivar la bala
                break;  // Salir del bucle tras manejar la colisión
            }
        }

        // Colisiones de la bala con muros normales (rebote)
        for (auto& wall : walls) {
            if (bullet.sprite.getGlobalBounds().intersects(wall.getBounds())) {
                sf::FloatRect wallBounds = wall.getBounds();
                sf::FloatRect bulletBounds = bullet.sprite.getGlobalBounds();

                // Calcular solapamiento en ambos ejes
                float overlapX = std::min(bulletBounds.left + bulletBounds.width, wallBounds.left + wallBounds.width) 
                               - std::max(bulletBounds.left, wallBounds.left);
                float overlapY = std::min(bulletBounds.top + bulletBounds.height, wallBounds.top + wallBounds.height) 
                               - std::max(bulletBounds.top, wallBounds.top);

                // Determinar el eje de colisión dominante y hacer rebotar la bala
                if (overlapX < overlapY) {
                    bullet.velocity.x = -bullet.velocity.x;  // Rebote horizontal
                } else {
                    bullet.velocity.y = -bullet.velocity.y;  // Rebote vertical
                }

                bullet.collisionCount++;
                break;  // Salir del bucle tras manejar la colisión
            }
        }

        // Limitar el número de colisiones
        if (bullet.collisionCount >= 8) {
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

        // Verificar si algún jugador ha sido destruido y restaurar su vida
        if (player1.estaDestruido()) {
            std::cout << "Jugador 1 ha sido destruido. Reviviendo..." << std::endl;
            player1.vidas = 3;
        }
        if (player2.estaDestruido()) {
            std::cout << "Jugador 2 ha sido destruido. Reviviendo..." << std::endl;
            player2.vidas = 3;
        }
    }

    // Eliminar las balas inactivas
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& bullet) { return !bullet.isActive; }), bullets.end());

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


// Renderizado de la escena del juego
void Game::render() {
    window.clear();
    player1.draw(window);
    player2.draw(window);

    for (const auto& wall : destructibleWalls) {
        wall.draw(window);
    }

    for (const auto& wall : walls) {
        wall.draw(window);
    }

    for (auto& bullet : bullets) {
        if (bullet.isActive) {
            bullet.draw(window);
        }
    }

    window.display();
}

// Disparar balas
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

// Dibuja las vidas de los tanques
void drawLives(sf::RenderWindow& window, const std::vector<Tank>& tanks, sf::Font& font) {
    for (const auto& tank : tanks) {
        if (!tank.estaDestruido()) {
            sf::Text text;
            text.setFont(font);
            text.setString("Vidas: " + std::to_string(tank.vidas));
            text.setCharacterSize(16);
            text.setFillColor(sf::Color::White);
            text.setPosition(tank.sprite.getPosition().x - 20, tank.sprite.getPosition().y - 50);
            window.draw(text);
        }
    }
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}
