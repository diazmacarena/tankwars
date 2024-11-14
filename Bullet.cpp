#include "Bullet.h"
#include "Tank.h"

// Constructor de la clase Bullet
Bullet::Bullet(const std::string& textureFile, float startX, float startY, sf::Vector2f direction, Tank* shooter)
    : isActive(true), collisionCount(0), owner(shooter) {
    if (!texture.loadFromFile(textureFile)) {
        std::cerr << "Error: No se pudo cargar la textura de la bala " << textureFile << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setScale(0.05f, 0.05f);  // Cambiar los factores de escala

    // Posicionar la bala en el cañón (a la derecha del tanque, centrado verticalmente)
    float angle = shooter->sprite.getRotation() * 3.1416f / 180.0f;  // Convierte grados a radianes
    float xOffset = std::cos(angle) * (shooter->sprite.getLocalBounds().width / 2);  // Posicionar a la derecha del tanque
    float yOffset = std::sin(angle) * (shooter->sprite.getLocalBounds().height / 2); // Centrar en la mitad vertical
    sprite.setPosition(startX + xOffset, startY + yOffset);

    // Normalizar la dirección y multiplicar por la velocidad
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) {
        direction /= length;  // Normalize
    }
    velocity = direction * 1.5f;  // Velocidad de la bala
}

// Movimiento de la bala
void Bullet::update() {
    if (isActive) {
        sprite.move(velocity);
    }
}

// Dibujar la bala
void Bullet::draw(sf::RenderWindow &window) {
    if (isActive) {
        window.draw(sprite);
    }
}

// Detectar colisiones con otro tanque, ignorando el tanque que disparó la bala
bool Bullet::checkCollision(const sf::Sprite &target) {
    // Verificamos que la bala esté activa y que no esté colisionando con el tanque que la disparó
    return isActive && &target != &owner->sprite && sprite.getGlobalBounds().intersects(target.getGlobalBounds());
}
