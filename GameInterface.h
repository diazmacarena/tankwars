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
    int currentOption;                 // Opción seleccionada en el menú
    std::vector<sf::Text> menuOptions; // Opciones del menú
    sf::Font font;                     // Fuente para todo el texto

    int player1TankType; // Tipo de tanque del jugador 1: 0 = Tank, 1 = TankEscopeta
    int player2TankType; // Tipo de tanque del jugador 2: 0 = Tank, 1 = TankEscopeta
};

#endif // GAME_INTERFACE_H
