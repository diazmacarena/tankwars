#include "Wall.h"

// Constructor: inicializa el tamaño, posición y color del muro
Wall::Wall(float width, float height, float posX, float posY, const sf::Color& color) {
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(posX, posY);
    shape.setFillColor(color);
}

// Dibuja el muro en la ventana proporcionada
void Wall::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

// Devuelve los límites del muro para detección de colisiones
const sf::FloatRect Wall::getBounds() const {
    return shape.getGlobalBounds();
}

