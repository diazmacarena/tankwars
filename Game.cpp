#include "Game.h"
#include <cmath>
#include <fstream>
#include <iostream>

// Tamaño de cada celda en el laberinto
const int TILE_SIZE = 40;



Game::Game() 
    : window(sf::VideoMode(1920, 1080), "Tanks Multiplayer"),
      player1("tank1.png", 200, 150),
      player2("tank2.png", 1600, 800) {
    cargarNivel(mapFile);  // Cargar el nivel desde un archivo
}

Game::Game(const std::string& nivel) : window(sf::VideoMode(1920,1080), "Tanks Multiplayer"),
    player1("tank1.png", 200, 150), player2("tank2.png",1600,800){
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
            if (line[col] == '1') {  // '1' representa un muro
                sf::RectangleShape muro(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                muro.setPosition(col * TILE_SIZE, row * TILE_SIZE);
                muro.setFillColor(sf::Color::White);  // Color del muro
                muros.push_back(muro);  // Agregar muro al vector de muros
            }
        }
        row++;
    }
}

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

void Game::moverTanque(Tank &player, float speed) {
    float angleRadians = degreesToRadians(player.sprite.getRotation());
    float dx = std::cos(angleRadians) * speed;
    float dy = std::sin(angleRadians) * speed;

    player.move(dx, dy);

    for (const auto& muro : muros) {
        if (player.sprite.getGlobalBounds().intersects(muro.getGlobalBounds())) {
            sf::FloatRect muroBounds = muro.getGlobalBounds();
            sf::FloatRect tankBounds = player.sprite.getGlobalBounds();

            // Verificar si la colisión es en X o Y
            if ((tankBounds.left + tankBounds.width > muroBounds.left && 
                 tankBounds.left < muroBounds.left + muroBounds.width)) {
                // Colisión en el eje X
                player.move(-dx, 0);
            }

            if ((tankBounds.top + tankBounds.height > muroBounds.top && 
                 tankBounds.top < muroBounds.top + muroBounds.height)) {
                // Colisión en el eje Y
                player.move(0, -dy);
            }
            break;
        }
    }
}

void Game::update() {
    for (auto& bullet : bullets) {
        if (!bullet.isActive) continue;

        bullet.update();

        // Colisiones de la bala con los muros
for (const auto& muro : muros) {
    if (bullet.sprite.getGlobalBounds().intersects(muro.getGlobalBounds())) {
        sf::FloatRect muroBounds = muro.getGlobalBounds();
        sf::FloatRect bulletBounds = bullet.sprite.getGlobalBounds();

        // Calcular solapamiento en ambos ejes
        float overlapX = std::min(bulletBounds.left + bulletBounds.width, muroBounds.left + muroBounds.width) 
                       - std::max(bulletBounds.left, muroBounds.left);
        float overlapY = std::min(bulletBounds.top + bulletBounds.height, muroBounds.top + muroBounds.height) 
                       - std::max(bulletBounds.top, muroBounds.top);

        // Determinar el eje de colisión dominante
        if (overlapX < overlapY) {
            // Colisión en el eje X (rebote horizontal)
            bullet.velocity.x = -bullet.velocity.x;
        } else {
            // Colisión en el eje Y (rebote vertical)
            bullet.velocity.y = -bullet.velocity.y;
        }

        bullet.collisionCount++;
        break; // Salir del bucle tras manejar la colisión
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
            player1.vidas = 3; // Restablecer las vidas de jugador 1
        }
        if (player2.estaDestruido()) {
            std::cout << "Jugador 2 ha sido destruido. Reviviendo..." << std::endl;
            player2.vidas = 3; // Restablecer las vidas de jugador 2
        }
    }

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

void Game::render() {
    window.clear();
    player1.draw(window);
    player2.draw(window);

    // Dibujar cada muro en el laberinto
    for (auto& muro : muros) {
        window.draw(muro);
    }

    // Dibuja cada bala activa
    for (auto &bullet : bullets) {
        if (bullet.isActive) {
            bullet.draw(window);
        }
    }

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

void updateGame(std::vector<Bullet>& bullets, std::vector<Tank>& tanks) {
    for (auto& bullet : bullets) {
        if (!bullet.isActive) continue; // Ignorar balas inactivas

        for (auto& tank : tanks) {
            if (!tank.estaDestruido() && bullet.sprite.getGlobalBounds().intersects(tank.sprite.getGlobalBounds())) {
                bullet.isActive = false; // Desactivar la bala tras el impacto
                tank.restarVida();       // Reducir una vida al tanque

                if (tank.estaDestruido()) {
                    std::cout << "Tanque destruido!\n";
                }

                break; // Una bala solo afecta a un tanque
            }
        }
    }
}


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

