#include "Portal.h"

// Constructor
Portal::Portal(const sf::Vector2f& posA, const sf::Vector2f& posB)
    : positionA(posA), positionB(posB) {}

// Getter para la posición del portal A
sf::Vector2f Portal::getPositionA() const {
    return positionA;
}

// Getter para la posición del portal B
sf::Vector2f Portal::getPositionB() const {
    return positionB;
}

// Verifica si un objeto está en el portal A
bool Portal::isInPortalA(const sf::Vector2f& objectPosition) const {
    // Compara si el objeto está cerca de la posición del portal A (un margen de tolerancia)
    const float tolerance = 5.0f; // Ajusta la tolerancia según el tamaño del objeto
    return std::abs(objectPosition.x - positionA.x) < tolerance &&
           std::abs(objectPosition.y - positionA.y) < tolerance;
}

// Retorna la posición del portal B
sf::Vector2f Portal::teleportToB() const {
    return positionB;
}