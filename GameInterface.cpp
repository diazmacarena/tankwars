#include "GameInterface.h"
#include <string>

GameInterface::GameInterface() : currentOption(0) {
    // Carga de la fuente
    font.loadFromFile("Extra Days.ttf"); // Asegúrate de que el archivo de fuente esté en el mismo directorio o especifica la ruta correcta.

    // Opciones del menú
    std::vector<std::string> options = {"Seleccion de niveles", "Como se juega", "Salir"};
    for (size_t i = 0; i < options.size(); ++i) {
        sf::Text text(options[i], font, 80);
        text.setPosition(500, 200 + i * 200);
        text.setFillColor(i == currentOption ? sf::Color::Green : sf::Color::White);
        menuOptions.push_back(text);
    }
}

void GameInterface::run() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "TankWars Menu");

    while (window.isOpen()) {
        handleEvents(window);
        window.clear();
        drawMenu(window);
        window.display();
    }
}

void GameInterface::drawMenu(sf::RenderWindow &window) {
    // Actualiza el color de las opciones según la selección
    for (size_t i = 0; i < menuOptions.size(); ++i) {
        menuOptions[i].setFillColor(i == currentOption ? sf::Color::Green : sf::Color::White);
        window.draw(menuOptions[i]);
    }
}

void GameInterface::handleEvents(sf::RenderWindow &window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) {
                // Cambiar la selección hacia arriba
                currentOption = (currentOption - 1 + menuOptions.size()) % menuOptions.size();
            } else if (event.key.code == sf::Keyboard::Down) {
                // Cambiar la selección hacia abajo
                currentOption = (currentOption + 1) % menuOptions.size();
            } else if (event.key.code == sf::Keyboard::Enter) {
                // Ejecutar la opción seleccionada
                if (currentOption == 0) {
                    selectLevel(window);
                } else if (currentOption == 1) {
                    showInstructions(window);
                } else if (currentOption == 2) {
                    window.close(); // Opción "Salir"
                }
            }
        }
    }
}

void GameInterface::selectLevel(sf::RenderWindow &window) {
    int level = 1; // Empieza en el nivel 1

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    level = (level > 1) ? level - 1 : 10;
                } else if (event.key.code == sf::Keyboard::Down) {
                    level = (level < 10) ? level + 1 : 1;
                } else if (event.key.code == sf::Keyboard::Enter) {
                    // Selecciona el archivo de mapa según el nivel
                    std::string mapFile = "nivel" + std::to_string(level) + ".txt";
                    Game juego(mapFile);
                    juego.run();  // Inicia el juego con el nivel seleccionado
                    return;  // Regresa al menú después de ejecutar el juego
                }
            }
        }

        window.clear();

        // Muestra el nivel actual seleccionado
        sf::Text levelText("Selecciona Nivel: " + std::to_string(level), font, 50);
        levelText.setPosition(700, 450);
        levelText.setFillColor(sf::Color::Green);
        window.draw(levelText);

        window.display();
    }
}

void GameInterface::showInstructions(sf::RenderWindow &window) {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                return; // Volver al menú al presionar Enter
            }
        }

        window.clear();

        sf::Text instructionsText1(
            "Tanque 1: W A S D <\npara moverse",
            font, 30
        );
        instructionsText1.setPosition(100, 200);
        instructionsText1.setFillColor(sf::Color::White);
        window.draw(instructionsText1);

        sf::Text instructionsText2("Tanque2: Usa las flechas \npara moverse" , font , 30);
        instructionsText2.setPosition(500, 200);
        instructionsText2.setFillColor(sf::Color::White);
        window.draw(instructionsText2);
        

        sf::Text instructionsText3(
            "Presiona la tecla enter para volver" , font , 30
        );
        instructionsText3.setPosition(500, 800);
        instructionsText3.setFillColor(sf::Color::White);
        window.draw(instructionsText3);

        window.display();
    }
}
