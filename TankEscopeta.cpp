#include "TankEscopeta.h"
#include <iostream>

// Constructor de TankEscopeta
TankEscopeta::TankEscopeta(const std::string& textureFile, float initialX, float initialY) 
    : Tank(textureFile, initialX, initialY), cartucho(4, 15.0f) {}

// Método para disparar proyectiles
void TankEscopeta::disparar(std::vector<sf::Vector2f>& direcciones) {
    if (cartucho.disparar(direcciones)) {
        std::cout << "Disparando proyectiles dispersos...\n";
    } else {
        std::cout << "Sin balas en el cartucho.\n";
    }
}

// Método para obtener las balas restantes
int TankEscopeta::getBalasRestantes() const {
    return cartucho.getBalasRestantes();
}