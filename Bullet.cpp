#include "Bullet.h"
#include "Tank.h"

// Constructor de la clase Bullet
Bullet::Bullet(const sf::Texture& texture, float startX, float startY, sf::Vector2f direction, Tank* shooter)
    : isActive(true), collisionCount(0), owner(shooter) {
    sprite.setTexture(texture);
    sprite.setScale(0.07f, 0.07f);
    
    //Lugar de donde sale la bala segun se mueva el tanque
    
    float angle = shooter->sprite.getRotation() * 3.1416f / 180;
    float xOffset = std::cos(angle) * (shooter->sprite.getLocalBounds().width / 2.5f);
    float yOffset = std::sin(angle) * (shooter->sprite.getLocalBounds().height / 2.5f);
    sprite.setPosition(startX + xOffset, startY + yOffset);

    // Obtener la direccion utilizando sqrt y dar la velocidad de la bala
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) {
        direction /= length;
    }
    velocity = direction * 1.1f;
}


// Actualiza los valores que no sin constantes
void Bullet::actualizar() {
    if (isActive) {
        sprite.move(velocity);
    }
}

// Dibujar la bala
void Bullet::dibujar(sf::RenderWindow &window) {
    if (isActive) {
        window.draw(sprite);
    }
}

// Detectar colisión con otro tanque
bool Bullet::colisionTanque(const sf::Sprite &target) {
    if (sprite.getGlobalBounds().intersects(target.getGlobalBounds())) {
        isActive = false; // Desactivar la bala tras colisión
        return true;
    }
    return false;
}

// Detectar colisión con un muro y ajustar dirección
void Bullet::colisionMuro(const sf::FloatRect &wallBounds) {
    sf::FloatRect bulletBounds = sprite.getGlobalBounds();

    // Verificar colisión en X
    if (bulletBounds.left < wallBounds.left || 
        bulletBounds.left + bulletBounds.width > wallBounds.left + wallBounds.width) {
        reboteX(); // Invertir dirección en X
        collisionCount++;
    }

    // Verificar colisión en Y
    if (bulletBounds.top < wallBounds.top || 
        bulletBounds.top + bulletBounds.height > wallBounds.top + wallBounds.height) {
        reboteY(); // Invertir dirección en Y
        collisionCount++;
    }
}

// Invertir dirección en X
void Bullet::reboteX() {
    velocity.x = -velocity.x; 
}

// Invertir dirección en Y
void Bullet::reboteY() {
    velocity.y = -velocity.y;
}
