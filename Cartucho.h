#ifndef CARTUCHO_H
#define CARTUCHO_H

#include <SFML/Graphics.hpp>
#include <vector>

class Cartucho {
private:
    int balasRestantes; //Balas que nos quedan
    float dispersion;   //Angulo de dispersion
public:
    // Constructor con parametros por defecto
    Cartucho(int balas = 4, float dispersion = 15.0f);
    
    // Verificar si se puede disparar y en que direccion segun el angulo del tanque
    bool disparar(std::vector<sf::Vector2f>& direcciones, float baseAngle);
    
    // Devuele las balas que le quedan al tanque
    int getBalasRestantes() const;
};

#endif
