#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

class GameInterface {
public:
    GameInterface();
    void run();

private:
    void drawMenu(sf::RenderWindow &window);
    void handleEvents(sf::RenderWindow &window);
    void selectLevel(sf::RenderWindow &window);
    void showInstructions(sf::RenderWindow &window);

    // Variables para el menú
    sf::Font font;
    std::vector<sf::Text> menuOptions;
    int currentOption;

    // Variables para los jugadores
    int player1TankType;
    int player2TankType;

    // Variables para el título animado
    sf::Text Title;                // Agregado
    sf::Clock CambioColorClock;    // Agregado
    std::vector<sf::Color> titleColors; // Agregado
    int indexColors;               // Agregado

    // Sonidos
    sf::SoundBuffer moveBuffer;
    sf::Sound moveSound;
    sf::SoundBuffer selectBuffer;
    sf::Sound selectSound;
    sf::SoundBuffer gameStartBuffer;
    sf::Sound gameStartSound;
};

#endif // GAMEINTERFACE_H
