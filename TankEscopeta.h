#ifndef TANKESCOPETA_H
#define TANKESCOPETA_H

#include "Tank.h"
#include "Cartucho.h"
#include <vector>

class TankEscopeta : public Tank {
private:
    Cartucho cartucho; // Contiene las balas y comportamiento de disparo

public:
    TankEscopeta(const std::string& textureFile, float initialX, float initialY);

    void disparar(std::vector<sf::Vector2f>& direcciones); // Método para disparar balas
    int getBalasRestantes() const; // Método para consultar las balas restantes
};

#endif