#ifndef DESTRUCTIBLE_WALL_H
#define DESTRUCTIBLE_WALL_H

#include "Wall.h"

// Clase que hereda de wall 
class DestructibleWall : public Wall {
private:
    
    bool destroyed; //Bool que verificara si el muro sigue en pie o esta destruido

public:
 
    DestructibleWall(float ancho, float largo, float posX, float posY, const sf::Color& color);
    
   
    void dibujar(sf::RenderWindow& window) const override; // Dibuja el muro si no está destruido

    
    void impacto();// Marca el muro como destruido al recibir un impacto
    
    
    bool isDestroyed() const;// Verifica si el muro está destruido
};

#endif // DESTRUCTIBLE_WALL_H
