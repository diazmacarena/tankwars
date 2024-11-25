#ifndef BULLET_H
#define BULLET_H
//Librerias incluidas
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <string>
//Buena practica de programacion, evita las dependencias circulares a la hora de compilar
class Tank;
//Clase bala
class Bullet {
public:
    sf::Sprite sprite;  //Gaseosa :v
    sf::Texture texture; //Textura que vamos a subir para la bala
    sf::Vector2f velocity; // Vector 2D para manejar las posiciones (x,y)
    bool isActive;
    int collisionCount;  // Contador de colisiones
    Tank* owner;         // Puntero al tanque que disparó la bala

    // Declaración del constructor
    Bullet(const sf::Texture& texture, float startX, float startY, sf::Vector2f direction, Tank* shooter);    // Movimiento de la bala
    
    // Acutaliza la bala
    void update();

    // Dibujar la bala
    void draw(sf::RenderWindow &window);

    // Detectar colisiones con otro tanque, ignorando el tanque que disparó la bala
    bool checkCollision(const sf::Sprite &target);
    // Manejo de colisiones para verificar si un tanque ha tocado un muro
    void handleWallCollision(const sf::FloatRect &wallBounds);

private:

    // Rebote en los muros
    void reverseX();  // Invertir dirección en X
    void reverseY();  // Invertir dirección en Y
    
};

#endif
