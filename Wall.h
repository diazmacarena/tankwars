#ifndef WALL_H
#define WALL_H

#include <SFML/Graphics.hpp>

class Wall {
protected:
    sf::RectangleShape shape;

public:
    Wall() = default;
    Wall(float width, float height, float posX, float posY, const sf::Color& color);
    
    virtual void draw(sf::RenderWindow& window) const ; // Método virtual puro

    virtual const sf::FloatRect getBounds() const; // Método virtual (no puro)
    
    virtual ~Wall() = default; // Destructor virtual
};

#endif // WALL_H
