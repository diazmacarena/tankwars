#include "Wall.h"

// Constructor: inicializa el tamaño, posición y color del muro
Wall::Wall(float width, float height, float posX, float posY, const sf::Color& color) {
    // Agrega el tamaño del muro 
    shape.setSize(sf::Vector2f(width, height));
    // Posiona el muro
    shape.setPosition(posX, posY);
    // Le da color a los muros
    shape.setFillColor(color);
}

// Dibuja el muro
void Wall::dibujar(sf::RenderWindow& window) const {
    window.draw(shape);
}

// Devuelve los limites del objeto shape(muro)
const sf::FloatRect Wall::getBounds() const {
    return shape.getGlobalBounds();
}