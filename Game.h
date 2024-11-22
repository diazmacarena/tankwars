#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Tank.h"
#include "Bullet.h"
#include "Wall.h"
#include "DestructibleWall.h" // Incluir clase de muros destructibles
#include <vector>
#include <string>

class Game {
private:
    sf::RenderWindow window;
    Tank player1;
    Tank player2;
    std::vector<Bullet> bullets;
    std::string mapFile;
    sf::Texture bulletTexture;

    // Parámetros de juego
    const float movementSpeed = 1.0f;
    const float rotationSpeed = 0.3f;
    const float shootInterval = 0.25f;
    const float reloadTime = 3.0f;
    const int maxBullets = 3;
    int player1Bullets = 2;
    int player2Bullets = 2;

    sf::Clock shootClockPlayer1;
    sf::Clock shootClockPlayer2;
    sf::Clock reloadClockPlayer1;
    sf::Clock reloadClockPlayer2;

    std::vector<Wall> walls;                   // Vector para muros indestructibles
    std::vector<DestructibleWall> destructibleWalls; // Vector para muros destructibles

    float degreesToRadians(float degrees);
    void processEvents();
    void moverTanque(Tank &player, float speed);
    void update();
    void render();
    void shootBullet(Tank &player, int &bulletCount, sf::Clock &shootClock, sf::Clock &reloadClock);

    void cargarNivel(const std::string& filename); // Método para cargar los niveles y muros

public:
    Game();
    Game(const std::string& mapFile);
    void run();
};

#endif // GAME_H

