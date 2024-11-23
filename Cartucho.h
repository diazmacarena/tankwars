#ifndef CARTUCHO_H
#define CARTUCHO_H

#include <SFML/Graphics.hpp>
#include <vector>

class Cartucho {
private:
    int balasRestantes;  // Número de balas que tiene el cartucho
    float dispersión;    // Ángulo de dispersión de los proyectiles

public:
    Cartucho(int balas = 4, float dispersion = 15.0f); // Constructor por defecto
    bool disparar(std::vector<sf::Vector2f>& direcciones); // Genera direcciones de disparo
    int getBalasRestantes() const;
};

#endif