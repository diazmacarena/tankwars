#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Tank.h"
#include "TankEscopeta.h"
#include "Bullet.h"
#include "Wall.h"
#include "DestructibleWall.h"
#include "Portal.h"
#include <vector>
#include <string>

class Game {
private:
    sf::RenderWindow window;
    Tank* player1;
    Tank* player2;
    std::vector<Bullet> bullets;
    std::string mapFile;
    sf::Texture bulletTexture;

    // Parámetros de juego
    const float movementSpeed = 1.0f;   // Incrementado para mayor velocidad
    const float rotationSpeed = 0.5f;   // Incrementado para mayor velocidad de rotación
    const float shootInterval = 0.25f;
    const float reloadTime = 1.2f;
    const int maxBullets = 6;

    int player1Bullets = 5;
    int player2Bullets = 5;

    sf::Clock shootClockPlayer1;
    sf::Clock shootClockPlayer2;
    sf::Clock reloadClockPlayer1;
    sf::Clock reloadClockPlayer2;
    sf::Clock gameOverClock;

    std::vector<Wall> walls;
    std::vector<DestructibleWall> destructibleWalls;
    sf::Vector2f lastValidPositionPlayer1;
    sf::Vector2f lastValidPositionPlayer2;

    sf::Font font;
    sf::Text winnerText;
    bool gameOver;

    // Música y sonidos
    sf::Music backgroundMusic;
    sf::SoundBuffer winBuffer;
    sf::Sound winSound;
    sf::SoundBuffer takeDamageBuffer;
    sf::SoundBuffer destructionBuffer;
    sf::SoundBuffer bulletHitBuffer;
    sf::Sound bulletHitSound;

    float degreesToRadians(float degrees);
    void processEvents();
    void moverTanque(Tank &player, float speed, sf::Vector2f &lastpositions);
    void update();
    void render();
    void shootBullet(Tank &player, int &bulletCount, sf::Clock &shootClock, sf::Clock &reloadClock);
    void shootEscopeta(TankEscopeta &player, sf::Clock &shootClock);

    void cargarNivel(const std::string& filename);

public:  // 2 jugadores (1 Tank, 1 Tank)
    Game(int playersType, const std::string& mapFile);             // Para manejar combinaciones específicas
    void run();
    ~Game();
};

#endif // GAME_H
