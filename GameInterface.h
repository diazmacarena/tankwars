#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Game.h"

class GameInterface {
public:
    GameInterface();
    void run();

private:
    // Métodos auxiliares
    void drawMenu(sf::RenderWindow &window);
    void handleEvents(sf::RenderWindow &window);
    void selectLevel(sf::RenderWindow &window);
    void showInstructions(sf::RenderWindow &window);

    // Variables de estado
    int currentOption; // Indica la opción seleccionada en el menú
    std::vector<sf::Text> menuOptions; // Las opciones del menú
    sf::Font font; // Fuente para todo el texto
};

#endif // GAME_INTERFACE_H
