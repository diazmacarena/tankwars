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
    // Dibuja el menu principal del juego(elegir tanque, nivel, instrucciones)
    void drawMenu(sf::RenderWindow &window);
    // Manejo de eventos al presionar las flecas, eleccion de tanques, nivel, etc.
    void handleEvents(sf::RenderWindow &window);
    // Selecionar el nivel desea
    void selectLevel(sf::RenderWindow &window);
    // Muestra las instrucciones
    void showInstructions(sf::RenderWindow &window);

    // Variables para el menú
    sf::Font font;
    std::vector<sf::Text> menuOptions; // Vectorr para agregar las opciones del menú
    int currentOption;

    // Variables para los jugadores
    int player1TankType;
    int player2TankType;

    // Variables para el título animado
    sf::Text Title;                // Para hacer el titulo
    sf::Clock CambioColorClock;    // Intervalo de cambio de colores
    std::vector<sf::Color> titleColors; // Colores a alternar 
    int indexColors;               // Posicion del vector y el color respectivo

    // Sonidos
    sf::SoundBuffer moveBuffer;  // Para hacer el sonido de moverse
    sf::Sound moveSound;        // Subir el sonido al moverse entre opciones
    sf::SoundBuffer selectBuffer;   //Hacer el sonido de seleccionar
    sf::Sound selectSound;  // Subir sonido de seleccionar
    sf::SoundBuffer gameStartBuffer;    //Hacer el sonido cuando entras al selector
    sf::Sound gameStartSound;       //Subir el sonido de entrar al selector cuando el juego inicia
};

#endif
