#ifndef CARTUCHO_H
#define CARTUCHO_H

#include <SFML/Graphics.hpp>
#include <vector>

class Cartucho {
private:
    int balasRestantes;
    float dispersion;

public:
    Cartucho(int balas = 4, float dispersion = 15.0f);
    bool disparar(std::vector<sf::Vector2f>& direcciones, float baseAngle);

    int getBalasRestantes() const;
};

#endif
