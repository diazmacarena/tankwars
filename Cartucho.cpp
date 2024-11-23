#include "Cartucho.h"
#include <cmath>

// Constructor por defecto
Cartucho::Cartucho(int balas, float dispersion) : balasRestantes(balas), dispersión(dispersion) {}

// Método para disparar y generar direcciones de proyectiles
bool Cartucho::disparar(std::vector<sf::Vector2f>& direcciones) {
    if (balasRestantes > 0) {
        float angleStep = dispersión / 3; // Separa los proyectiles en un rango uniforme
        for (int i = -1; i <= 1; i++) {
            float angle = i * angleStep;
            float radianes = angle * (3.14159f / 180.0f);
            direcciones.push_back(sf::Vector2f(cos(radianes), sin(radianes)));
        }
        direcciones.push_back(sf::Vector2f(1, 0)); // Dirección central
        balasRestantes--;
        return true;
    }
    return false;
}

// Getter para obtener las balas restantes
int Cartucho::getBalasRestantes() const {
    return balasRestantes;
}