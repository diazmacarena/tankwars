#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Tank.h"
#include "Bullet.h"
#include "Wall.h"
#include <vector>

class Game {
private:
    sf::RenderWindow window;
    Tank player1;
    Tank player2;
    std::vector<Bullet> bullets;
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

    std::vector<sf::RectangleShape> muros;

    // Muros definidos como instancias de la clase Wall
    Wall muroIzquierdo;
    Wall muroDerecho;
    Wall muroSuperior;
    Wall muroInferior;

    float degreesToRadians(float degrees);
    void processEvents();
    void moverTanque(Tank &player, float speed);
    void update();
    void render();
    void shootBullet(Tank &player, int &bulletCount, sf::Clock &shootClock, sf::Clock &reloadClock);

    void cargarNivel(const std::string& filename);

public:
    Game();
    void run();
};

#endif // GAME_H
