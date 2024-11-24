#include "GameInterface.h"
#include <string>

#include "GameInterface.h"
#include <string>
#include <iostream>  // Para std::cerr

GameInterface::GameInterface() : currentOption(0), player1TankType(0), player2TankType(0) {
    // Carga de la fuente
    if (!font.loadFromFile("Extra Days.ttf")) {
        std::cerr << "Error al cargar la fuente Extra Days.ttf\n";
    }

    // Opciones del menú
    std::vector<std::string> options = {"Seleccion de niveles", "Como se juega", "Salir"};
    for (size_t i = 0; i < options.size(); ++i) {
        sf::Text text(options[i], font, 80);
        text.setPosition(500, 200 + i * 200);
        text.setFillColor(i == currentOption ? sf::Color::Green : sf::Color::White);
        menuOptions.push_back(text);
    }

    // Cargar los sonidos
    if (!moveBuffer.loadFromFile("move_interface.wav")) {
        std::cerr << "Error al cargar move_interface.wav\n";
    } else {
        moveSound.setBuffer(moveBuffer);
        moveSound.setVolume(50); // Ajustar volumen si es necesario
    }

    if (!selectBuffer.loadFromFile("select.wav")) {
        std::cerr << "Error al cargar select.wav\n";
    } else {
        selectSound.setBuffer(selectBuffer);
        selectSound.setVolume(50);
    }

    if (!gameStartBuffer.loadFromFile("game_start.wav")) {
        std::cerr << "Error al cargar game_start.wav\n";
    } else {
        gameStartSound.setBuffer(gameStartBuffer);
        gameStartSound.setVolume(70);
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
                int previousOption = currentOption;
                currentOption = (currentOption - 1 + menuOptions.size()) % menuOptions.size();
                if (currentOption != previousOption) {
                    // Reproducir sonido de movimiento
                    moveSound.play();
                }
            } else if (event.key.code == sf::Keyboard::Down) {
                int previousOption = currentOption;
                currentOption = (currentOption + 1) % menuOptions.size();
                if (currentOption != previousOption) {
                    // Reproducir sonido de movimiento
                    moveSound.play();
                }
            } else if (event.key.code == sf::Keyboard::Enter) {
                // Reproducir sonido de selección
                selectSound.play();
                if (currentOption == 0) {
                    // Reproducir sonido de inicio del juego
                    gameStartSound.play();
                    selectLevel(window);
                } else if (currentOption == 1) {
                    showInstructions(window);
                } else if (currentOption == 2) {
                    window.close();
                }
            }
        }
    }
}


void GameInterface::selectLevel(sf::RenderWindow &window) {
    int level = 1;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                bool moved = false; // Bandera para verificar si se debe reproducir el sonido de movimiento
                if (event.key.code == sf::Keyboard::Down) {
                    level = (level > 1) ? level - 1 : 10;
                    moved = true;
                } else if (event.key.code == sf::Keyboard::Up) {
                    level = (level < 10) ? level + 1 : 1;
                    moved = true;
                } else if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right) {
                    player2TankType = (player2TankType == 0) ? 1 : 0; // Cambiar tanque del jugador 2
                    moved = true;
                } else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::D) {
                    player1TankType = (player1TankType == 0) ? 1 : 0; // Cambiar tanque del jugador 1
                    moved = true;
                } else if (event.key.code == sf::Keyboard::Enter) {
                    // Reproducir sonido de selección
                    selectSound.play();

                    std::string mapFile = "nivel" + std::to_string(level) + ".txt";

                    // Configurar los tipos de tanque según las selecciones
                    int playersType = 0;
                    if (player1TankType == 0 && player2TankType == 0) {
                        playersType = 1; // Tank vs Tank
                    } else if (player1TankType == 0 && player2TankType == 1) {
                        playersType = 2; // Tank vs TankEscopeta
                    } else if (player1TankType == 1 && player2TankType == 0) {
                        playersType = 3; // TankEscopeta vs Tank
                    } else if (player1TankType == 1 && player2TankType == 1) {
                        playersType = 4; // TankEscopeta vs TankEscopeta
                    }

                    Game juego(playersType, mapFile);
                    juego.run();
                    return;
                }

                if (moved) {
                    // Reproducir sonido de movimiento
                    moveSound.play();
                }
            }
        }

        window.clear();

        // Mostrar selección de nivel
        sf::Text levelText("Selecciona Nivel: " + std::to_string(level), font, 50);
        levelText.setPosition(700, 300);
        levelText.setFillColor(sf::Color::Green);
        window.draw(levelText);

        // Mostrar tanque seleccionado para jugador 1
        sf::Text player1Text(
            "Jugador 1 (WASD): " + std::string(player1TankType == 0 ? "Tank" : "Tank Escopetin"),
            font, 40);
        player1Text.setPosition(300, 500);
        player1Text.setFillColor(sf::Color::White);
        window.draw(player1Text);

        // Mostrar tanque seleccionado para jugador 2
        sf::Text player2Text(
            "Jugador 2 (Flechas): " + std::string(player2TankType == 0 ? "Tank" : "Tank Escopetin"),
            font, 40);
        player2Text.setPosition(300, 900);
        player2Text.setFillColor(sf::Color::White);
        window.draw(player2Text);

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
                // Reproducir sonido de selección
                selectSound.play();
                return;
            }
        }

        window.clear();

        sf::Text instructionsText1("Tanque 1: W A S D para moverse", font, 30);
        instructionsText1.setPosition(100, 200);
        instructionsText1.setFillColor(sf::Color::White);
        window.draw(instructionsText1);

        sf::Text instructionsText2("Tanque 2: Usa las flechas para moverse", font, 30);
        instructionsText2.setPosition(500, 400);
        instructionsText2.setFillColor(sf::Color::White);
        window.draw(instructionsText2);

        sf::Text instructionsText3("Presiona Enter para volver", font, 30);
        instructionsText3.setPosition(500, 800);
        instructionsText3.setFillColor(sf::Color::White);
        window.draw(instructionsText3);

        window.display();
    }
}
