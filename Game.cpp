#include "Game.h"
#include <cmath>
#include <fstream>
#include <iostream>

// Tamaño de cada celda en el laberinto

// Constructor para casos 1-2, 1-4, 3-2, 3-4
Game::Game(int playersType, const std::string& nivel)
    : window(sf::VideoMode(1920, 1080), "Tanks Multiplayer"), gameOver(false)  {
    // Switch case para los tanques que sean
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
    textoGanador.setFont(font);
    textoGanador.setCharacterSize(80);
    textoGanador.setFillColor(sf::Color::Yellow);
    textoGanador.setStyle(sf::Text::Bold);

    // Cargar la textura de la bala
    if (!bulletTexture.loadFromFile("bullet.png")) {
        std::cerr << "Error: No se pudo cargar la textura de la bala bullet.png" << std::endl;
    }

    // Cargar la música de fondo
    if (!backgroundMusic.openFromFile("Musica-de-fondo.ogg")) {
        std::cerr << "Error: No se pudo cargar Musica-de-fondo.ogg" << std::endl;
    } else {
        backgroundMusic.setLoop(true); // Hacer que la música se repita
        backgroundMusic.play();
        backgroundMusic.setVolume(15); // Bajar el volumen de la música de fondo
    }

    // Cargar el sonido de victoria
    if (!winBuffer.loadFromFile("win.wav")) {
        std::cerr << "Error: No se pudo cargar win.wav" << std::endl;
    } else {
        winSound.setBuffer(winBuffer);
        winSound.setVolume(100); // Priorizar el sonido de victoria
    }

    // Cargar los sonidos de daño y destrucción
    if (!takeDamageBuffer.loadFromFile("take_damage.wav")) {
        std::cerr << "Error: No se pudo cargar take_damage.wav" << std::endl;
    }

    if (!destructionBuffer.loadFromFile("tank_destruction.wav")) {
        std::cerr << "Error: No se pudo cargar tank_destruction.wav" << std::endl;
    }
    if (!bulletHitBuffer.loadFromFile("bullet_hit.wav")) {
        std::cerr << "Error: No se pudo cargar bullet_hit.wav" << std::endl;
    } else {
        bulletHitSound.setBuffer(bulletHitBuffer);
        bulletHitSound.setVolume(70);
    }

    // Asignar los sonidos a los tanques con ->
    player1->setTakeDamageSound(takeDamageBuffer);
    player1->setDestructionSound(destructionBuffer);

    player2->setTakeDamageSound(takeDamageBuffer);
    player2->setDestructionSound(destructionBuffer);

    // Asignar la textura de la bala
    player1->setBulletTexture(bulletTexture);
    player2->setBulletTexture(bulletTexture);

    cargarNivel(nivel);
}

//Destructor para los jugadores
Game::~Game(){
    delete player1;
    delete player2;
}
//Funcion para convertir grados a radianes para la rotación 
float Game::degreesToRadians(float degrees) {
    return degrees * (3.1416f / 180.0f);
}
const int TILE_SIZE = 40;
// Función para cargar el nivel desde un archivo
void Game::cargarNivel(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error al abrir el archivo de nivel: " << filename << std::endl;
        return;
    }
    //Explicación:Los niveles son matrices, si ve que es 1 o 2 dibuja un muro segun sea su tipo y lo pone con pushback
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
void Game::manejarEventos() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }

    // Control del jugador 1
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) 
        moverTanque(*player1, velocidadDeMovimiento, ultimaPosicionValidaPlayer1);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) 
        moverTanque(*player1, -velocidadDeMovimiento, ultimaPosicionValidaPlayer1);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) 
        player1->rotar(-velocidadDeRotacion);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) 
        player1->rotar(velocidadDeRotacion);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        player1->disparar(bullets, shootClockPlayer1, player1Balas, intervaloDisparos, recarga, reloadClockPlayer1);
    }

    // Control del jugador 2
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) 
        moverTanque(*player2, velocidadDeMovimiento, ultimaPosicionValidaPlayer2);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        moverTanque(*player2, -velocidadDeMovimiento, ultimaPosicionValidaPlayer2);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        player2->rotar(-velocidadDeRotacion);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
        player2->rotar(velocidadDeRotacion);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        player2->disparar(bullets, shootClockPlayer2, player2Balas, intervaloDisparos, recarga, reloadClockPlayer2);
    }
}

void Game::moverTanque(Tank &player, float speed, sf::Vector2f &lastValidPosition) {
    float angRadianes = degreesToRadians(player.sprite.getRotation());
    float dx = std::cos(angRadianes) * speed;
    float dy = std::sin(angRadianes) * speed;

    // Intentar mover el tanque temporalmente para verificar colisiones
    player.mover(dx, dy);

    auto calcularSobrePosicion= [&](const sf::FloatRect& tankBounds, const sf::FloatRect& wallBounds) -> float {
        float sobreX = std::max(0.0f, std::min(tankBounds.left + tankBounds.width, wallBounds.left + wallBounds.width) - 
                                      std::max(tankBounds.left, wallBounds.left));
        float sobreY = std::max(0.0f, std::min(tankBounds.top + tankBounds.height, wallBounds.top + wallBounds.height) - 
                                      std::max(tankBounds.top, wallBounds.top));

        float sobreArea = sobreX * sobreY;
        float tankArea = tankBounds.width * tankBounds.height;

        return (sobreArea / tankArea) * 100;  // Porcentaje de solapamiento
    };

    auto handleCollision = [&](const Wall& muro) {
        sf::FloatRect muroBounds = muro.getBounds();
        sf::FloatRect tankBounds = player.sprite.getGlobalBounds();
        
        float porcentajeSobrePosicion = calcularSobrePosicion(tankBounds, muroBounds);

        // Si el tanque tiene un solapamiento del 20% o más, restaurarlo a su última posición sin colisión
        if (porcentajeSobrePosicion >= 20.0f) {
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
void Game::renderizar() {
    //Limpia la ventana antes de hacer algo
    window.clear();

    if (gameOver) {
        // Mostrar el mensaje del ganador
        window.draw(textoGanador);
    } else {
        window.draw(player1->sprite);
        window.draw(player2->sprite);

        // Dibujar balas
        for (const auto &bullet : bullets) {
            if (bullet.isActive) {
                window.draw(bullet.sprite);
            }
        }
        // Dibujar paredes
        for (const auto &wall : walls) {
            wall.dibujar(window);
}

// Dibujar muros destructibles
        for (const auto &destructibleWall : destructibleWalls) {
            if (!destructibleWall.isDestroyed()) {
                 destructibleWall.dibujar(window);
    }
}
}

    window.display();
}


// Disparar balas
void Game::dispararBala(Tank &player, int &bulletCount, sf::Clock &shootClock, sf::Clock &reloadClock) {
    if (bulletCount > 0) {
        float angleRadians = degreesToRadians(player.sprite.getRotation());
        sf::Vector2f direction(std::cos(angleRadians), std::sin(angleRadians));

        bullets.emplace_back(bulletTexture, player.sprite.getPosition().x, player.sprite.getPosition().y, direction, &player);

        bulletCount--;
        shootClock.restart();
        reloadClock.restart();
    }
}

void Game::correr() {
    while (window.isOpen()) {
        manejarEventos();
        actualizar();
        renderizar();
    }
}
void Game::actualizar() {
    if (gameOver) {
        // Verificar si han pasado 4 segundos
        if (tiempoMostrarGanador.getElapsedTime().asSeconds() >= 4.0f) {
            window.close(); // Cerrar la ventana del juego
        }
        return; // No continuar actualizando el juego
    }

    for (auto& bullet : bullets) {
        if (!bullet.isActive) continue;

        bullet.actualizar();

        // Colisiones de la bala con muros destructibles
        for (auto& wall : destructibleWalls) {
            // Solo verificar colisión si el muro no está destruido
            if (!wall.isDestroyed() && bullet.sprite.getGlobalBounds().intersects(wall.getBounds())) {
                wall.impacto();  // Destruir el muro destructible
                bullet.isActive = false;
                if (bulletHitSound.getStatus() == sf::Sound::Playing) {
                    bulletHitSound.stop();
                    }
                bulletHitSound.play(); 
                break;  // Salir del bucle tras manejar la colisión
            }
        }

        // Colisiones de la bala con muros normales (rebote)
        for (auto& wall : walls) {
            if (bullet.sprite.getGlobalBounds().intersects(wall.getBounds())) {
                sf::FloatRect wallBounds = wall.getBounds();
                sf::FloatRect bulletBounds = bullet.sprite.getGlobalBounds();

                // Calcular solapamiento en ambos ejes
                float choqueX = std::min(bulletBounds.left + bulletBounds.width, wallBounds.left + wallBounds.width) 
                               - std::max(bulletBounds.left, wallBounds.left);
                float choqueY = std::min(bulletBounds.top + bulletBounds.height, wallBounds.top + wallBounds.height) 
                               - std::max(bulletBounds.top, wallBounds.top);

                // Determinar el eje de colisión dominante y hacer rebotar la bala
                if (choqueX < choqueY) {
                    bullet.velocity.x = -bullet.velocity.x;  // Rebote horizontal
                } else {
                    bullet.velocity.y = -bullet.velocity.y;  // Rebote vertical
                }
                if (bulletHitSound.getStatus() == sf::Sound::Playing) {
                    bulletHitSound.stop();
                    }
                bulletHitSound.play();
                bullet.collisionCount++;
                break;  // Salir del bucle tras manejar la colisión
            }
        }

        // Limitar el número de colisiones
        if (bullet.collisionCount >= 6) {
            bullet.isActive = false;
        }

        // Verificar colisiones con los tanques y restar vidas
        if (bullet.colisionTanque(player1->sprite)) {
            player1->restarVida();
            std::cout << "Jugador 1 ha recibido un impacto, vidas restantes: " << player1->vidas << std::endl;
            bullet.isActive = false;
        }
        if (bullet.colisionTanque(player2->sprite)) {
            player2->restarVida();
            std::cout << "Jugador 2 ha recibido un impacto, vidas restantes: " << player2->vidas << std::endl;
            bullet.isActive = false;
            }
        

    }

    // Eliminar las balas inactivas
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& bullet) { return !bullet.isActive; }), bullets.end());

    // Recargar balas de cada jugador si es necesario
    if (reloadClockPlayer1.getElapsedTime().asSeconds() >= recarga && player1Balas < balasMaximas) {
        player1Balas++;
        reloadClockPlayer1.restart();
    }
    if (reloadClockPlayer2.getElapsedTime().asSeconds() >= recarga && player2Balas < balasMaximas) {
        player2Balas++;
        reloadClockPlayer2.restart();
    }
    if (player1->estaDestruido() || player2->estaDestruido()) {
        if (!gameOver) { // Solo ejecutar una vez
            gameOver = true;
            tiempoMostrarGanador.restart(); // Reiniciar el reloj para contar los 5 segundos

            // Detener la música de fondo
            backgroundMusic.stop();

            // Reproducir sonido de victoria
            winSound.play();

            // Determinar quién ganó
            if (player1->estaDestruido() && !player2->estaDestruido()) {
                textoGanador.setString("El Jugador 2 Gana");
            } else if (!player1->estaDestruido() && player2->estaDestruido()) {
                textoGanador.setString("El Jugador 1 Gana");
            } else {
                textoGanador.setString("Empate");
            }

            // Centrar el texto en la pantalla
            sf::FloatRect textRect = textoGanador.getLocalBounds();
            textoGanador.setOrigin(textRect.left + textRect.width / 2.0f,
                                 textRect.top + textRect.height / 2.0f);
            textoGanador.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
        }
    }

}