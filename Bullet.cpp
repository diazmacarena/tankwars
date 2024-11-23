#include "Bullet.h"
#include "Tank.h"

// Constructor de la clase Bullet
Bullet::Bullet(const std::string& textureFile, float startX, float startY, sf::Vector2f direction, Tank* shooter)
    : isActive(true), collisionCount(0), owner(shooter) {
    if (!texture.loadFromFile(textureFile)) {
        std::cerr << "Error: No se pudo cargar la textura de la bala " << textureFile << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setScale(0.07f, 0.07f);  // Cambiar los factores de escala
    // Posicionar la bala en el cañón (a la derecha del tanque, centrado verticalmente)
    float angle = shooter->sprite.getRotation() * 3.1416f / 180.0f;  // Convierte grados a radianes
    float xOffset = std::cos(angle) * (shooter->sprite.getLocalBounds().width / 2);  // Posicionar a la derecha del tanque
    float yOffset = std::sin(angle) * (shooter->sprite.getLocalBounds().height / 2); // Centrar en la mitad vertical
    sprite.setPosition(startX + xOffset - 90 , startY + yOffset );

    // Normalizar la dirección y multiplicar por la velocidad
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) {
        direction /= length;  // Normalize
    }
    velocity = direction * 1.1f;  // Velocidad de la bala
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

// Detectar colisión con otro objeto
bool Bullet::checkCollision(const sf::Sprite &target) {
    if (sprite.getGlobalBounds().intersects(target.getGlobalBounds())) {
        isActive = false; // Desactivar la bala tras colisión
        return true;
    }
    return false;
}

// Detectar colisión con un muro y ajustar dirección
void Bullet::handleWallCollision(const sf::FloatRect &wallBounds) {
    sf::FloatRect bulletBounds = sprite.getGlobalBounds();

    // Verificar colisión en X
    if (bulletBounds.left < wallBounds.left || 
        bulletBounds.left + bulletBounds.width > wallBounds.left + wallBounds.width) {
        reverseX(); // Invertir dirección en X
        collisionCount++;
    }

    // Verificar colisión en Y
    if (bulletBounds.top < wallBounds.top || 
        bulletBounds.top + bulletBounds.height > wallBounds.top + wallBounds.height) {
        reverseY(); // Invertir dirección en Y
        collisionCount++;
    }
}

// Invertir dirección en X
void Bullet::reverseX() {
    velocity.x = -velocity.x;
}

// Invertir dirección en Y
void Bullet::reverseY() {
    velocity.y = -velocity.y;
}
