#include "TankEscopeta.h"
#include "Bullet.h"
#include <iostream>
#include <cmath>

// Constructor de TankEscopeta
#include "TankEscopeta.h"
#include <iostream>

TankEscopeta::TankEscopeta(const std::string& textureFile, float initialX, float initialY)
    : Tank(textureFile, initialX, initialY), cartucho(4, 15.0f) {
    speed = 1.0f; // Aseguramos que la velocidad sea la misma que la del tanque base
    std::cout << "TankEscopeta creado correctamente con la textura " << textureFile << std::endl;
}

void TankEscopeta::disparar(std::vector<Bullet>& bullets, sf::Clock &shootClock, int &bulletCount,
                         const float shootInterval, const float reloadTime, sf::Clock &reloadClock) {
    if (shootClock.getElapsedTime().asSeconds() >= shootInterval && cartucho.getBalasRestantes() > 0) {
        float baseAngle = sprite.getRotation();
        std::vector<sf::Vector2f> direcciones;

        if (cartucho.disparar(direcciones, baseAngle)) {
            for (const auto &direccion : direcciones) {
                bullets.emplace_back(*bulletTexture, sprite.getPosition().x, sprite.getPosition().y, direccion, this);
            }
            // Reinicia los relojes del disparo y recarga
            shootClock.restart();
            reloadClock.restart();
        }
    }
}




// Método para obtener las balas restantes
int TankEscopeta::getBalasRestantes() const {
    return cartucho.getBalasRestantes();
}

