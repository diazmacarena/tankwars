#ifndef WALL_H
#define WALL_H

#include <SFML/Graphics.hpp>

class Wall {
private:
    sf::RectangleShape shape;

public:
    Wall() = default;
    Wall(float width, float height, float posX, float posY, const sf::Color& color);
    void draw(sf::RenderWindow& window) const;
    const sf::FloatRect getBounds() const; // Devuelve los límites para detectar colisiones
};

#endif // WALL_H
