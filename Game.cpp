#include "Game.h"
#include <cmath>
#include <fstream>
#include <iostream>

// Tamaño de cada celda en el laberinto
const int TILE_SIZE = 40;

// Constructor para casos 1-2, 1-4, 3-2, 3-4
Game::Game(int playersType, const std::string& nivel)
    : window(sf::VideoMode(1920, 1080), "Tanks Multiplayer"), gameOver(false)  {
    // Initialize players based on playersType
    switch (playersType) {
        case 1: // Tank vs Tank
            player1 = new Tank("Tank1.png", 200, 150);
            player2 = new Tank("Tank2.png", 1600, 800);
            break;
        case 2: // Tank vs TankEscopeta
            player1 = new Tank("Tank1.png", 200, 150);
            player2 = new TankEscopeta("Tank2.png", 1600, 800);
            break;
        case 3: // TankEscopeta vs Tank
            player1 = new TankEscopeta("Tank1.png", 200, 150);
            player2 = new Tank("Tank2.png", 1600, 800);
            break;
        case 4: // TankEscopeta vs TankEscopeta
            player1 = new TankEscopeta("Tank1.png", 200, 150);
            player2 = new TankEscopeta("Tank2.png", 1600, 800);
            break;
        default:
            std::cerr << "Tipo de jugadores no válido. Usando configuración predeterminada (1 Tank, 1 TankEscopeta).\n";
            player1 = new Tank("Tank1.png", 200, 150);
            player2 = new TankEscopeta("Tank2.png", 1600, 800);
            break;
    }

    // Cargar la fuente
    if (!font.loadFromFile("Extra Days.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente Extra Days.ttf" << std::endl;
    }

    // Configurar el texto del ganador
    winnerText.setFont(font);
    winnerText.setCharacterSize(80);
    winnerText.setFillColor(sf::Color::Yellow);
    winnerText.setStyle(sf::Text::Bold);

    // Load the bullet texture
    if (!bulletTexture.loadFromFile("bullet.png")) {
        std::cerr << "Error: No se pudo cargar la textura de la bala bullet.png" << std::endl;
    }

    // Set the bullet texture for the tanks
    player1->setBulletTexture(bulletTexture);
    player2->setBulletTexture(bulletTexture);

    cargarNivel(nivel);
}

Game::~Game(){
    delete player1;
    delete player2;
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
        moverTanque(*player1, movementSpeed, lastValidPositionPlayer1);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) 
        moverTanque(*player1, -movementSpeed, lastValidPositionPlayer1);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) 
        player1->rotate(-rotationSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) 
        player1->rotate(rotationSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        player1->shoot(bullets, shootClockPlayer1, player1Bullets, shootInterval, reloadTime, reloadClockPlayer1);
    }

    // Control del jugador 2
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) 
        moverTanque(*player2, movementSpeed, lastValidPositionPlayer2);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        moverTanque(*player2, -movementSpeed, lastValidPositionPlayer2);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        player2->rotate(-rotationSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
        player2->rotate(rotationSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        player2->shoot(bullets, shootClockPlayer2, player2Bullets, shootInterval, reloadTime, reloadClockPlayer2);
    }
}

    void Game::moverTanque(Tank &player, float speed, sf::Vector2f &lastValidPosition) {
    float angleRadians = degreesToRadians(player.sprite.getRotation());
    float dx = std::cos(angleRadians) * speed;
    float dy = std::sin(angleRadians) * speed;

    // Intentar mover el tanque temporalmente para verificar colisiones
    player.move(dx, dy);

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

        // Si el tanque tiene un solapamiento del 12% o más, restaurarlo a su última posición sin colisión
        if (overlapPercentage >= 20.0f) {
            player.sprite.setPosition(lastValidPosition);
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

    // Solo actualizar la última posición válida si no hay colisión
    if (!isColliding) {
        lastValidPosition = player.sprite.getPosition();
    } else {
        // Si hay una colisión, revertir a la última posición válida y no permitir movimiento hacia el muro
        player.sprite.setPosition(lastValidPosition);
    }
}


// Renderizado de la escena del juego
void Game::render() {
    
    window.clear();

    if (gameOver) {
        // Mostrar el mensaje del ganador
        window.draw(winnerText);
    } else {
        // ... código existente para dibujar el juego ...
        window.draw(player1->sprite);
        window.draw(player2->sprite);

        // Dibujar balas
        for (const auto &bullet : bullets) {
            if (bullet.isActive) {
                window.draw(bullet.sprite);
            }
        }

        // Dibujar paredes
        // Dibujar paredes
        for (const auto &wall : walls) {
            wall.draw(window);
}

// Dibujar muros destructibles
        for (const auto &destructibleWall : destructibleWalls) {
            if (!destructibleWall.isDestroyed()) {
                 destructibleWall.draw(window);
    }
}
}

    window.display();
}


// Disparar balas
void Game::shootBullet(Tank &player, int &bulletCount, sf::Clock &shootClock, sf::Clock &reloadClock) {
    if (bulletCount > 0) {
        float angleRadians = degreesToRadians(player.sprite.getRotation());
        sf::Vector2f direction(std::cos(angleRadians), std::sin(angleRadians));

        bullets.emplace_back(bulletTexture, player.sprite.getPosition().x, player.sprite.getPosition().y, direction, &player);

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
void Game::update() {

    if (gameOver) {
        // Verificar si han pasado 5 segundos
        if (gameOverClock.getElapsedTime().asSeconds() >= 5.0f) {
            window.close(); // Cerrar la ventana del juego
        }
        return; // No continuar actualizando el juego
    }

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
        // Verificar colisiones con los tanques y restar vidas
        if (bullet.checkCollision(player1->sprite)) {
            player1->restarVida();
            std::cout << "Jugador 1 ha recibido un impacto, vidas restantes: " << player1->vidas << std::endl;
            bullet.isActive = false;
        }
        if (bullet.checkCollision(player2->sprite)) {
            player2->restarVida();
            std::cout << "Jugador 2 ha recibido un impacto, vidas restantes: " << player2->vidas << std::endl;
            bullet.isActive = false;
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
   if (player1->estaDestruido() || player2->estaDestruido()) {
    if (!gameOver) { // Solo ejecutar una vez
        gameOver = true;
        gameOverClock.restart(); // Reiniciar el reloj para contar los 5 segundos

        // Determinar quién ganó
        if (player1->estaDestruido() && !player2->estaDestruido()) {
            winnerText.setString("¡Jugador 2 Gana!");
        } else if (!player1->estaDestruido() && player2->estaDestruido()) {
            winnerText.setString("¡Jugador 1 Gana!");
        } else {
            winnerText.setString("¡Empate!");
        }

        // Centrar el texto en la pantalla
        sf::FloatRect textRect = winnerText.getLocalBounds();
        winnerText.setOrigin(textRect.left + textRect.width / 2.0f,
                             textRect.top + textRect.height / 2.0f);
        winnerText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

        // Detener cualquier música o sonidos si es necesario
    }
}

}

