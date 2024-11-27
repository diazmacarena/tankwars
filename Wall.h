#ifndef WALL_H
#define WALL_H

#include <SFML/Graphics.hpp>

class Wall {
protected:
    sf::RectangleShape shape; // Objeto (rectangulo) que representa un muro 

public:
    Wall(float width, float height, float posX, float posY, const sf::Color& color);
    
    virtual void dibujar(sf::RenderWindow& window) const ; // Método virtual 
    virtual const sf::FloatRect getBounds() const; // Método virtual (no puro)
    
};

#endif // WALL_H


