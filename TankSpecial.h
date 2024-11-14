#ifndef TANKSPECIAL_H
#define TANKSPECIAL_H

#include <SFML/Graphics.hpp>
#include "Tank.h"

class TankSpecial : public Tank {
private:
    sf::Texture bulletTexture;
    sf::Sprite bulletSprite1, bulletSprite2;
    bool isShooting1, isShooting2;
    bool canShootAgain;  // Para controlar si puede disparar nuevamente

public:
    // Constructor
    TankSpecial(const std::string& textureFile, const std::string& bulletTextureFile, float initialX, float initialY);

    // Método para disparar dos balas seguidas
    void shoot();

    // Actualizar la posición de las balas
    void updateBullets();

    // Renderizar el tanque y las balas
    void draw(sf::RenderWindow &window) override;

    // restar vida al tanque cuando es impactado
    void restarVida() override;
};

#endif
