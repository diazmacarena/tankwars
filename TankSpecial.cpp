#include "TankSpecial.h"
#include <iostream>

// Constructor
TankSpecial::TankSpecial(const std::string& textureFile, const std::string& bulletTextureFile, float initialX, float initialY)
    : Tank(textureFile, initialX, initialY), isShooting1(false), isShooting2(false), canShootAgain(true) {
    // Cargar textura de la bala
    if (!bulletTexture.loadFromFile(bulletTextureFile)) {
        std::cerr << "Error: No se pudo cargar la textura de la bala" << std::endl;
    }

    bulletSprite1.setTexture(bulletTexture);
    bulletSprite2.setTexture(bulletTexture);
    bulletSprite1.setScale(0.1f, 0.1f);  // Tamaño de la bala
    bulletSprite2.setScale(0.1f, 0.1f);  // Tamaño de la bala

    bulletSprite1.setOrigin(bulletSprite1.getLocalBounds().width / 2, bulletSprite1.getLocalBounds().height / 2);  // Centrar la bala
    bulletSprite2.setOrigin(bulletSprite2.getLocalBounds().width / 2, bulletSprite2.getLocalBounds().height / 2);  // Centrar la bala
}

// Método para disparar dos balas seguidas
void TankSpecial::shoot() {
    if (canShootAgain) {
        canShootAgain = false;
        isShooting1 = true;
        isShooting2 = true;

        // Establecer la posición de las balas (una a la izquierda y otra a la derecha del tanque)
        bulletSprite1.setPosition(sprite.getPosition() + sf::Vector2f(-10, 0));  // Bala 1 a la izquierda
        bulletSprite2.setPosition(sprite.getPosition() + sf::Vector2f(10, 0));   // Bala 2 a la derecha

        // Restar un punto de vida cada vez que se dispare
        restarVida();
    }
}

// Actualizar la posición de las balas
void TankSpecial::updateBullets() {
    if (isShooting1) {
        bulletSprite1.move(5.0f * direction.x, 5.0f * direction.y);
        if (bulletSprite1.getPosition().x < 0 || bulletSprite1.getPosition().x > 800 || 
            bulletSprite1.getPosition().y < 0 || bulletSprite1.getPosition().y > 600) {
            isShooting1 = false;  // Detener la bala cuando se salga de la ventana
        }
    }

    if (isShooting2) {
        bulletSprite2.move(5.0f * direction.x, 5.0f * direction.y);
        if (bulletSprite2.getPosition().x < 0 || bulletSprite2.getPosition().x > 800 || 
            bulletSprite2.getPosition().y < 0 || bulletSprite2.getPosition().y > 600) {
            isShooting2 = false;  // Detener la bala cuando se salga de la ventana
        }
    }

    // Rehabilitar el disparo después de un pequeño retraso
    if (!isShooting1 && !isShooting2) {
        canShootAgain = true;
    }
}

// Renderizar el tanque y las balas
void TankSpecial::draw(sf::RenderWindow &window) {
    Tank::draw(window);  // Dibuja el tanque

    if (isShooting1) {
        window.draw(bulletSprite1);  // Dibuja la primera bala
    }
    if (isShooting2) {
        window.draw(bulletSprite2);  // Dibuja la segunda bala
    }
}

// Método para restar vida al tanque cuando es impactado
void TankSpecial::restarVida() {
    if (vidas > 0) {
        vidas--;
    }
}
