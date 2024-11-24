#ifndef TANK_H
#define TANK_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <string>
class Bullet;

class Tank {
public:
    sf::Sprite sprite;
    sf::Texture texture;
    float speed;
    sf::Vector2f direction;
    int vidas;  // Atributo para almacenar las vidas del tanque

    Tank();
    // Constructor de la clase Tank
    Tank(const std::string& textureFile, float initialX, float initialY);

    // Funciones virtuales para ser sobrescritas por clases derivadas
    virtual void rotate(float angle);
    virtual void move(float offsetX, float offsetY);
    virtual void draw(sf::RenderWindow &window);

    // Método virtual para restar vida
    virtual void restarVida();

    virtual void shoot(std::vector<Bullet>& bullets, sf::Clock &shootClock,
                         int &bulletCount, const float shootInterval, 
                         const float reloadTime, sf::Clock &reloadClock);

    // Verificar si el tanque ha sido destruido
    bool estaDestruido() const;

    void ocultar();
protected:
    static float degreesToRadians(float degres);
};

#endif
