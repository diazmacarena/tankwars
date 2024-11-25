#include "DestructibleWall.h"

// Constructor que inicializa el muro como no destruido que hereda del constructor de wall
DestructibleWall::DestructibleWall(float width, float height, float posX, float posY, const sf::Color& color)
    : Wall(width, height, posX, posY, color), destroyed(false) {}

// Dibuja el muro si no está destruido
void DestructibleWall::draw(sf::RenderWindow& window) const {
    if (!destroyed) {
        window.draw(shape);
    }
}

// Marca el muro como destruido al recibir un impacto
void DestructibleWall::hit() {
    destroyed = true;
}

// Devuelve true si el muro está destruido
bool DestructibleWall::isDestroyed() const {
    return destroyed;
}
