#ifndef DESTRUCTIBLE_WALL_H
#define DESTRUCTIBLE_WALL_H

#include "Wall.h"

class DestructibleWall : public Wall {
private:
    bool destroyed;

public:
    DestructibleWall(float width, float height, float posX, float posY, const sf::Color& color);
    
    // Dibuja el muro si no está destruido
    void draw(sf::RenderWindow& window) const override;

    // Marca el muro como destruido al recibir un impacto
    void hit();
    
    // Verifica si el muro está destruido
    bool isDestroyed() const;
};

#endif // DESTRUCTIBLE_WALL_H
