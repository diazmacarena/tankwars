#ifndef INTERFACE_H
#define INTERFACE_H

#include <SFML/Graphics.hpp>
#include "Game.h"

class INterface {
private:
    sf::RenderWindow ventana;
    sf::RectangleShape barraProgreso;
    sf::Text textoCargando;
    sf::Font fuente;
    float progreso;

public:
    // Constructor
    INterface();

    // Ejecutar la pantalla de carga y luego iniciar el juego
    void ejecutar();

private:
    // Procesar los eventos (como cerrar la ventana)
    void procesarEventos();

    // Actualizar la barra de progreso
    void actualizar();

    // Renderizar la pantalla de carga
    void renderizar();

    // Método para iniciar el juego después de la pantalla de carga
    void iniciarJuego();
};

#endif // INTERFACE_H
