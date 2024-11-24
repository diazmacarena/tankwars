#include "Cartucho.h"
#include <cmath>

Cartucho::Cartucho(int balas, float dispersion)
    : balasRestantes(balas), dispersion(dispersion) {}


bool Cartucho::disparar(std::vector<sf::Vector2f>& direcciones, float baseAngle) {
    if (balasRestantes <= 0) {
        return false;
    }
    balasRestantes--;

    int numProjectiles = 5; // Número de proyectiles por disparo
    float angleStep = dispersion / (numProjectiles - 1);

    for (int i = 0; i < numProjectiles; ++i) {
        float angle = baseAngle - (dispersion / 2) + i * angleStep;
        float radianAngle = angle * (3.14159265f / 180.0f);
        sf::Vector2f direction(std::cos(radianAngle), std::sin(radianAngle));
        direcciones.push_back(direction);
    }

    return true;
}


int Cartucho::getBalasRestantes() const {
    return balasRestantes;
}
