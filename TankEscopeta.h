#ifndef TANKESCOPETA_H
#define TANKESCOPETA_H

#include "Tank.h"
#include "Cartucho.h"
#include <vector>

class Bullet;

class TankEscopeta : public Tank {
private:
    Cartucho cartucho; // Contiene las balas y comportamiento de disparo, usando la clase Cartucho

public:
    // Constructor 
    TankEscopeta(const std::string& textureFile, float initialX, float initialY);
    
    // Metodo para disparar las balas 
    void disparar(std::vector<Bullet>& bullets, sf::Clock &shootClock, int &bulletCount,
               const float shootInterval, const float reloadTime, sf::Clock &reloadClock) override; 
    
    // Método para consultar las balas restantes
    int getBalasRestantes() const; 
};

#endif