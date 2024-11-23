#ifndef PORTAL_H
#define PORTAL_H

#include <SFML/Graphics.hpp>

class Portal {
private:
    sf::Vector2f positionA; // Posición del portal A
    sf::Vector2f positionB; // Posición del portal B

public:
    // Constructor
    Portal(const sf::Vector2f& posA, const sf::Vector2f& posB);

    // Getter para la posición del portal A
    sf::Vector2f getPositionA() const;

    // Getter para la posición del portal B
    sf::Vector2f getPositionB() const;

    // Método para verificar si un objeto está en el portal A
    bool isInPortalA(const sf::Vector2f& objectPosition) const;

    // Método para obtener la posición de teletransporte (portal B)
    sf::Vector2f teleportToB() const;
};

#endif // PORTAL_H