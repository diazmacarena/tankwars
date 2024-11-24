#ifndef TANKESCOPETA_H
#define TANKESCOPETA_H

#include "Tank.h"
#include "Cartucho.h"
#include <vector>

class Bullet;

class TankEscopeta : public Tank {
private:
    Cartucho cartucho; // Contiene las balas y comportamiento de disparo

public:
    TankEscopeta(const std::string& textureFile, float initialX, float initialY);

   void shoot(std::vector<Bullet>& bullets, sf::Clock &shootClock, int &bulletCount,
               const float shootInterval, const float reloadTime, sf::Clock &reloadClock) override; // Método para disparar balas
    int getBalasRestantes() const; // Método para consultar las balas restantes
};

#endif