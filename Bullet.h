#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <string>

class Tank;

class Bullet {
public:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f velocity;
    bool isActive;
    int collisionCount;  // Contador de colisiones
    Tank* owner;         // Puntero al tanque que disparó la bala

    // Declaración del constructor
    Bullet(const sf::Texture& texture, float startX, float startY, sf::Vector2f direction, Tank* shooter);    // Movimiento de la bala
    void update();

    // Dibujar la bala
    void draw(sf::RenderWindow &window);

    // Detectar colisiones con otro tanque, ignorando el tanque que disparó la bala
    bool checkCollision(const sf::Sprite &target);

    void handleWallCollision(const sf::FloatRect &wallBounds);

private:
    void reverseX();  // Invertir dirección en X
    void reverseY();  // Invertir dirección en Y
    
};

#endif
